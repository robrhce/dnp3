/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#include <boost/test/unit_test.hpp>

#include "TestHelpers.h"
#include "BufferHelpers.h"

#include <opendnp3/APDU.h>
#include <opendnp3/ObjectHeader.h>
#include <opendnp3/ObjectWriteIterator.h>
#include <opendnp3/DNPConstants.h>

#include <opendnp3/DataTypes.h>
#include <opendnp3/QualityMasks.h>

#include <openpal/ToHex.h>

#include <queue>

using namespace opendnp3;
using namespace openpal;
using namespace std;


BOOST_AUTO_TEST_SUITE(APDUWriting)
BOOST_AUTO_TEST_CASE(WriteIIN)
{
	auto hex = "C4 02 50 01 00 07 07 00";

	APDU frag;
	frag.SetFunction(FunctionCode::WRITE);
	frag.SetControl(true, true, false, false, 4);

	Group80Var1* pObj = Group80Var1::Inst();
	ObjectWriteIterator i = frag.WriteContiguous(pObj, 7, 7);

	pObj->Write(*i, 7, 7, false);
	++i;

	BOOST_REQUIRE(i.IsEnd());		
	BOOST_REQUIRE_EQUAL(hex, toHex(frag.ToReadOnly(), true));
}

BOOST_AUTO_TEST_CASE(ResponseWithDataAndFlags)
{
	auto hex = "E3 81 96 00 02 01 28 01 00 00 00 01 02 01 28 01 00 01 00 01 02 01 28 01 00 02 00 01 02 01 28 01 00 03 00 01 20 02 28 01 00 00 00 01 00 00 20 02 28 01 00 01 00 01 00 00 01 01 01 00 00 03 00 00 1E 02 01 00 00 01 00 01 00 00 01 00 00";

	APDU frag;
	frag.SetFunction(FunctionCode::RESPONSE);
	frag.SetControl(true, true, true, false, 3);
	IINField iin;
	iin.Set(IINBit::DEVICE_RESTART);
	iin.Set(IINBit::NEED_TIME);
	iin.Set(IINBit::CLASS1_EVENTS);
	iin.Set(IINBit::CLASS2_EVENTS);
	frag.SetIIN(iin);

	{
		Group2Var1* pObj = Group2Var1::Inst();
		IndexedWriteIterator i = frag.WriteIndexed(pObj, 1, QualifierCode::UINT16_CNT_UINT16_INDEX); //300 max index forces the index bit with to 2 octet
		i.SetIndex(0);
		pObj->mFlag.Set(*i, BQ_ONLINE);
		++i;
		BOOST_REQUIRE(i.IsEnd());

		BOOST_REQUIRE_EQUAL(frag.Size(), 12);
		i = frag.WriteIndexed(pObj, 1, QualifierCode::UINT16_CNT_UINT16_INDEX);
		i.SetIndex(1);
		pObj->mFlag.Set(*i, BQ_ONLINE);
		++i;
		BOOST_REQUIRE(i.IsEnd());

		BOOST_REQUIRE_EQUAL(frag.Size(), 20);

		i = frag.WriteIndexed(pObj, 1, QualifierCode::UINT16_CNT_UINT16_INDEX);
		i.SetIndex(2);
		pObj->mFlag.Set(*i, BQ_ONLINE);
		++i;
		BOOST_REQUIRE(i.IsEnd());

		BOOST_REQUIRE_EQUAL(frag.Size(), 28);

		i = frag.WriteIndexed(pObj, 1, QualifierCode::UINT16_CNT_UINT16_INDEX);
		i.SetIndex(3);
		pObj->mFlag.Set(*i, BQ_ONLINE);

		++i;
		BOOST_REQUIRE(i.IsEnd());
	}
		

	{
		// 20 02 28 01 00 00 00 01 00 00 - Group32Var2 , 2 octet count w/ 2 octet index
		Group32Var2* pObj = Group32Var2::Inst();
		IndexedWriteIterator i = frag.WriteIndexed(pObj, 1, 300);
		i.SetIndex(0);
		pObj->mFlag.Set(*i, AQ_ONLINE);
		pObj->mValue.Set(*i, 0);
		++i;
		BOOST_REQUIRE(i.IsEnd());
		BOOST_REQUIRE_EQUAL(frag.Size(), 46);

		// same again with index of 1
		i = frag.WriteIndexed(pObj, 1, 300);
		i.SetIndex(1);
		pObj->mFlag.Set(*i, AQ_ONLINE);
		pObj->mValue.Set(*i, 0);
		++i;
		BOOST_REQUIRE(i.IsEnd());
	}

	BOOST_REQUIRE_EQUAL(frag.Size(), 56);

	{
		// 01 01 01 00 00 03 00 00 - Obj1Var1 with 2octet start/stop indices
		Group1Var1* pObj = Group1Var1::Inst();
		ObjectWriteIterator i = frag.WriteContiguous(pObj, 0, 3, QualifierCode::UINT16_START_STOP);

		pObj->Write(*i, 0, 0, false);
		++i;
		BOOST_REQUIRE_FALSE(i.IsEnd());

		pObj->Write(*i, 0, 1, false);
		++i;
		BOOST_REQUIRE_FALSE(i.IsEnd());

		pObj->Write(*i, 0, 2, false);
		++i;
		BOOST_REQUIRE_FALSE(i.IsEnd());

		pObj->Write(*i, 0, 3, false);
		++i;
		BOOST_REQUIRE(i.IsEnd());
	}	

	// 1E 02 01 00 00 01 00 01 00 00 01 00 00 - Obj30Var2 with 2octet start/stop indices 00->01

	{
		Group30Var2* pObj = Group30Var2::Inst();
		ObjectWriteIterator i = frag.WriteContiguous(pObj, 0, 1, QualifierCode::UINT16_START_STOP);

		pObj->mFlag.Set(*i, AQ_ONLINE);
		pObj->mValue.Set(*i, 0);
		++i;
		BOOST_REQUIRE_FALSE(i.IsEnd());

		pObj->mFlag.Set(*i, AQ_ONLINE);
		pObj->mValue.Set(*i, 0);
		++i;
		BOOST_REQUIRE(i.IsEnd());
	}
	
	BOOST_REQUIRE_EQUAL(hex, toHex(frag.ToReadOnly(), true));
}

BOOST_AUTO_TEST_CASE(Obj1Var1Write)
{
	auto hex = "C0 81 00 00 01 01 00 00 05 2A";

	APDU frag;
	frag.SetFunction(FunctionCode::RESPONSE);
	IINField iin;
	frag.SetIIN(iin);
	frag.SetControl(true, true);

	BitfieldObject* pObj = Group1Var1::Inst();
	ObjectWriteIterator owi = frag.WriteContiguous(Group1Var1::Inst(), 0, 5);//, BWM_AUTO, true);

	for(int i = 0; i < 6; ++i) {
		pObj->Write(*owi, 0, i, (i % 2) == 1);
		++owi;
	}
	
	BOOST_REQUIRE_EQUAL(hex, toHex(frag.ToReadOnly(), true));
}

BOOST_AUTO_TEST_CASE(Obj32Var2Write)
{
	APDU frag;
	frag.SetFunction(FunctionCode::RESPONSE);
	IINField iin;
	frag.SetIIN(iin);
	frag.SetControl(true, true);

	IndexedWriteIterator iter = frag.WriteIndexed(Group32Var2::Inst(), 1, QualifierCode::UINT32_CNT_UINT32_INDEX);
	iter.SetIndex(2);
	Analog a(3, AQ_RESTART);
	Group32Var2::Inst()->Write(*iter, a);

	auto expected = "C0 81 00 00 20 02 39 01 00 00 00 02 00 00 00 02 03 00";	
	BOOST_REQUIRE_EQUAL(expected, toHex(frag.ToReadOnly(), true));
}

BOOST_AUTO_TEST_CASE(SingleSetpoint)
{
	APDU frag;
	frag.SetFunction(FunctionCode::RESPONSE);
	IINField f;
	frag.SetIIN(f);
	frag.SetControl(true, true, false, false, 0);

	IndexedWriteIterator itr = frag.WriteIndexed(Group41Var3::Inst(), 1, QualifierCode::UINT8_CNT_UINT8_INDEX);
	AnalogOutputFloat32 s(100.0);

	itr.SetIndex(1);
	Group41Var3::Inst()->Write(*itr, s);	

	// group 41 var 4, count = 1, index = 0, value = 100.0, status = CommandStatus::SUCCESS
	BOOST_REQUIRE_EQUAL("C0 81 00 00 29 03 17 01 01 00 00 C8 42 00", toHex(frag.ToReadOnly(), true));
}

BOOST_AUTO_TEST_CASE(DoubleSetpoint)
{
	APDU frag;
	frag.SetFunction(FunctionCode::RESPONSE);
	IINField f;
	frag.SetIIN(f);
	frag.SetControl(true, true, false, false, 0);

	IndexedWriteIterator itr = frag.WriteIndexed(Group41Var4::Inst(), 1, QualifierCode::UINT8_CNT_UINT8_INDEX);
	AnalogOutputDouble64 s(100.0);

	itr.SetIndex(1);

	Group41Var4::Inst()->Write(*itr, s);	

	// group 41 var 4, count = 1, index = 0, value = 100.0, status = CommandStatus::SUCCESS
	BOOST_REQUIRE_EQUAL("C0 81 00 00 29 04 17 01 01 00 00 00 00 00 00 59 40 00", toHex(frag.ToReadOnly(), true));
}

BOOST_AUTO_TEST_CASE(ClassPollRequest)
{
	auto hex = "C3 01 3C 02 06 3C 03 06 3C 04 06 3C 01 06";

	APDU frag;
	frag.SetFunction(FunctionCode::READ);
	frag.SetControl(true, true, false, false, 3);

	BOOST_REQUIRE_EQUAL(frag.Size(), 2);

	frag.DoPlaceholderWrite(Group60Var2::Inst());
	BOOST_REQUIRE_EQUAL(frag.Size(), 5);
	frag.DoPlaceholderWrite(Group60Var3::Inst());
	BOOST_REQUIRE_EQUAL(frag.Size(), 8);
	frag.DoPlaceholderWrite(Group60Var4::Inst());
	BOOST_REQUIRE_EQUAL(frag.Size(), 11);
	frag.DoPlaceholderWrite(Group60Var1::Inst());
	BOOST_REQUIRE_EQUAL(frag.Size(), 14);
	
	BOOST_REQUIRE_EQUAL(hex, toHex(frag.ToReadOnly(), true));
}

BOOST_AUTO_TEST_CASE(Confirm)
{
	auto hex = "C3 00";

	APDU frag;
	frag.SetFunction(FunctionCode::CONFIRM);
	frag.SetControl(true, true, false, false, 3);
	
	BOOST_REQUIRE_EQUAL(hex, toHex(frag.ToReadOnly(), true));
}

BOOST_AUTO_TEST_CASE(IndexRead)
{
	auto hex = "C0 01 01 02 17 03 01 03 05";

	APDU frag;
	frag.SetFunction(FunctionCode::READ);
	frag.SetControl(true, true);
	IndexedWriteIterator i = frag.WriteIndexed(Group1Var2::Inst(), 3, 255);

	i.SetIndex(1); ++i;
	i.SetIndex(3); ++i;
	i.SetIndex(5); ++i;

	BOOST_REQUIRE(i.IsEnd());

	BOOST_REQUIRE_EQUAL(hex, toHex(frag.ToReadOnly(), true));	
}

BOOST_AUTO_TEST_CASE(Sequence15Correct)
{
	auto hex = "CF 00";

	APDU frag;
	frag.SetFunction(FunctionCode::CONFIRM);
	frag.SetControl(true, true, false, false, 15);

	BOOST_REQUIRE_EQUAL(hex, toHex(frag.ToReadOnly(), true));
}

BOOST_AUTO_TEST_CASE(VirtualTerminalWrite)
{
	APDU frag;
	frag.SetFunction(FunctionCode::WRITE);
	frag.SetControl(true, true, false, false, 2);

	std::string str("hello");
	size_t outSize = str.size();
	int index = 0;

	IndexedWriteIterator i = frag.WriteIndexed(Group112Var0::Inst(), outSize, index);
	i.SetIndex(index);
	const uint8_t* ptr = reinterpret_cast<const uint8_t*>(str.c_str());
	Group112Var0::Inst()->Write(*i, outSize, ptr);
	
	BOOST_REQUIRE_EQUAL("C2 02 70 05 17 01 00 68 65 6C 6C 6F", toHex(frag.ToReadOnly(), true));
}

BOOST_AUTO_TEST_CASE(VirtualTerminalWriteMultipleIndices)
{
	APDU frag;
	frag.SetFunction(FunctionCode::WRITE);
	frag.SetControl(true, true, false, false, 2);

	// Write the first object
	std::string hello("hello");
	size_t outSize = hello.size();
	int index = 5;

	IndexedWriteIterator i = frag.WriteIndexed(Group112Var0::Inst(), outSize, index);
	i.SetIndex(index);
	const uint8_t* ptr = reinterpret_cast<const uint8_t*>(hello.c_str());
	Group112Var0::Inst()->Write(*i, outSize, ptr);

	// Write the second object
	std::string world("world");
	outSize = world.size();
	index = 16;

	i = frag.WriteIndexed(Group112Var0::Inst(), outSize, index);
	i.SetIndex(index);
	ptr = reinterpret_cast<const uint8_t*>(world.c_str());
	Group112Var0::Inst()->Write(*i, outSize, ptr);

	/* Now check to see if things are correct */	
	BOOST_REQUIRE_EQUAL("C2 02 70 05 17 01 05 68 65 6C 6C 6F 70 05 17 01 10 77 6F 72 6C 64", toHex(frag.ToReadOnly(), true));
}
BOOST_AUTO_TEST_SUITE_END()

/* vim: set ts=4 sw=4: */

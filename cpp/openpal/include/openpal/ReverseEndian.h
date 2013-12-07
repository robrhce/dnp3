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
#ifndef __OPENPAL_REVERSE_ENDIAN_H_
#define __OPENPAL_REVERSE_ENDIAN_H_

#include <cstdint>

#include "SerializationTemplates.h"

namespace openpal {

class UInt8Simple
{
public:

	inline static uint8_t Read(const uint8_t* apStart) 
	{
		return (*apStart);
	}

	inline static uint8_t ReadBuffer(ReadOnlyBuffer& arBuffer) 
	{
		auto ret = Read(arBuffer);
		arBuffer.Advance(Size);
		return ret;
	}

	inline static void Write(uint8_t* apStart, uint8_t aValue) 
	{
		*(apStart) = aValue;
	}

	const static size_t Size = 1;
	const static uint8_t Max;
	const static uint8_t Min;

	typedef uint8_t Type;
};

class UInt48LE
{
public:

	static int64_t Read(const uint8_t* apStart);	
	static void Write(uint8_t* apStart, int64_t aValue);

	inline static int64_t ReadBuffer(ReadOnlyBuffer& arBuffer) 
	{
		auto ret = Read(arBuffer);
		arBuffer.Advance(Size);
		return ret;
	}

	const static int64_t MAX = 281474976710655ULL; // 2^48 -1
	const static size_t Size = 6;
	typedef int64_t Type;
};

}

#endif

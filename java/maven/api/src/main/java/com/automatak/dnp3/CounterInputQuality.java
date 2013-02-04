/**
 * Copyright 2013 Automatak, LLC
 *
 * Licensed to Automatak, LLC (www.automatak.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. Automatak, LLC
 * licenses this file to you under the GNU Affero General Public License
 * Version 3.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 * http://www.gnu.org/licenses/agpl.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
package com.automatak.dnp3;


import java.util.Collections;
import java.util.HashSet;
import java.util.Set;

/**
 * Quality enumeration for counters
 */
public enum CounterInputQuality {
    /**
     * Set when the data is "good", meaning that rest of the system can trust the value
     */
    ONLINE(0x01),

    /**
     * the quality all points get before we have established communication (or populated) the point in a remote database
     */
    RESTART(0x02),

    /**
     *  Communication has been lost with the source of the data (after establishing contact)
     */
    COMM_LOST(0x04),

    /**
     *   the value is being forced to a "fake" value somewhere in the system
     */
    REMOTE_FORCED_DATA(0x08),

    /**
     * the value is being forced to a "fake" value on the original device
     */
    LOCAL_FORCED_DATA(0x10),

    /**
     * used to indicate that the counter filled up and rolled over, cleared automatically after reading
     */
    ROLLOVER(0x20),

    /**
     * indicates an unusual change in value
     */
    DISCONTINUITY(0x40),


    RESERVED (0x80);

    private final int id;

    private CounterInputQuality(int id)
    {
        this.id = id;
    }

    public static Set<CounterInputQuality> getValuesInBitField(byte bitfield)
    {
        Set<CounterInputQuality> set = new HashSet<CounterInputQuality>();
        if((bitfield & ONLINE.toInt()) != 0)  set.add(ONLINE);
        if((bitfield & RESTART.toInt()) != 0)  set.add(RESTART);
        if((bitfield & COMM_LOST.toInt()) != 0)  set.add(COMM_LOST);
        if((bitfield & REMOTE_FORCED_DATA.toInt()) != 0) set.add(REMOTE_FORCED_DATA);
        if((bitfield & LOCAL_FORCED_DATA.toInt()) != 0) set.add(LOCAL_FORCED_DATA);
        if((bitfield & ROLLOVER.toInt()) != 0) set.add(ROLLOVER);
        if((bitfield & DISCONTINUITY.toInt()) != 0) set.add(DISCONTINUITY);
        if((bitfield & RESERVED.toInt()) != 0) set.add(RESERVED);
        return Collections.unmodifiableSet(set);
    }

    /**
     * @return The underlying integer representation of the enum
     */
    public int toInt()
    {
        return id;
    }
}
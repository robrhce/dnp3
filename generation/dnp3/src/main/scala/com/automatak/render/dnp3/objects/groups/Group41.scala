package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects._

import FixedSizeField._


object Group41 extends ObjectGroup {
  def objects = List(Group41Var1, Group41Var2, Group41Var3, Group41Var4)
  def group: Byte = 41
}

object Group41Var1 extends FixedSize(Group41, 1)(value32, commandStatus) with ConversionToAnalogOutputInt32
object Group41Var2 extends FixedSize(Group41, 2)(value16, commandStatus) with ConversionToAnalogOutputInt16
object Group41Var3 extends FixedSize(Group41, 3)(float32, commandStatus) with ConversionToAnalogOutputFloat32
object Group41Var4 extends FixedSize(Group41, 4)(float64, commandStatus) with ConversionToAnalogOutputDouble64
package au.com.krynj.aoc.twentyfour.days

import au.com.krynj.aoc.framework.AoCDay
import au.com.krynj.aoc.util.AoCUtil
import au.com.krynj.aoc.util.AoCUtil.numberWords
import java.math.BigInteger

class DayOne: AoCDay {
    override fun run() {
        println(String.format("Task 1-1: %s", partOne()))
        println(String.format("Task 1-2: %s", partTwo()))
    }

    override fun getDay(): Int {
        return 1
    }

    fun partOne(): BigInteger {
        var lines = AoCUtil.readResourceFile("input_1.txt")

        var lineNumSum = BigInteger.ZERO

        for (line: String in lines)
        {
            lineNumSum = lineNumSum.add(lineNumber(line, false))
        }

        return lineNumSum
    }

    fun partTwo(): BigInteger {
        var lines = AoCUtil.readResourceFile("input_1.txt")

        var lineNumSum = BigInteger.ZERO

        for (line: String in lines)
        {
            lineNumSum = lineNumSum.add(lineNumber(line, true))
        }

        return lineNumSum
    }

    fun lineNumber(line: String, useWords: Boolean): BigInteger {
        var firstDigitLoc = line.indexOfFirst { it in AoCUtil.getStringDigits() }
        var firstDigit =  BigInteger(line[firstDigitLoc].toString())
        var lastDigitLoc = line.indexOfLast { it in AoCUtil.getStringDigits() }
        var lastDigit =  BigInteger(line[lastDigitLoc].toString())
        if (useWords) {
            val firstWords = numberWords.map { Pair(line.indexOf(it.second), AoCUtil.digitStringToInt(it.second)) }.filter { it.first != -1 }
            val lastWords = numberWords.map { Pair(line.lastIndexOf(it.second), AoCUtil.digitStringToInt(it.second)) }.filter { it.first != -1 }

            if (firstWords.isNotEmpty() && firstWords.minOf { it.first } < firstDigitLoc) {
                firstDigitLoc = firstWords.minBy { it.first }.first
                firstDigit = firstWords.minBy { it.first }.second!!
            }
            if (lastWords.isNotEmpty() && lastWords.maxOf { it.first } > lastDigitLoc) {
                lastDigitLoc = lastWords.maxBy { it.first }.first
                lastDigit = lastWords.maxBy { it.first }.second!!
            }
        }
        return BigInteger(firstDigit.toString() + lastDigit.toString())
    }
}
#pragma once

namespace Utils
{
    /// <summary>
    /// Takes an int and maps it into range: [low, high)
    /// </summary>
    /// <param name="num">input number</param>
    /// <param name="low">low bound inclusive</param>
    /// <param name="high">high bound exclusive</param>
    /// <returns></returns>
    static int BindRange(int num, int low, int high)
    {
        int range = high - low;
        return ((((num - low) % range) + range) % range) + low;
    }
}
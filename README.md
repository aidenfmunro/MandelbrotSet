# Mandelbrot Set

The Mandelbrot set is a complex mathematical set that is defined by iterating a simple mathematical formula. 
The set is famous for its intricate and visually stunning fractal patterns, which are self-similar at different scales.

![](img/mandelbrot.png)

## System specs

Processor: AMD Ryzen 7 5800HS with Radeon Graphics, 3201 Mhz, 8 Core(s), 16 Logical Processor(s)

Compiler: g++ 11.4.0

OS: Ubuntu 22.04 (WSL)

## Basic implementation

```
for each pixel (Px, Py) on the screen do
    x0 := scaled x coordinate of pixel
    y0 := scaled y coordinate of pixel

    x  := x0
    y  := y0

    r_max         := 10

    max_iteration := 256
    iteration     := 0
    
    while (x^2 + y^2 ≤ r_max^2 AND iteration < max_iteration) do
        x := x^2 - y^2 + x0
        y := 2 * x * y + y0

        iteration := iteration + 1

    color := calculate color

    plot(Px, Py, color)

```
The pseudcode above was rewritten in C.
 
Some simple optimization were applied related to loops and variables.

**Result:** ~ 825 * 10^9 ticks per 100 cycles.

## Vectorized implementation

SIMD (Single Instruction Multiple Data) instructions enable parallel processing of data, enabling multiple computations to be carried out simultaneously on different data elements. This can lead to faster execution and improved efficiency in tasks such as image processing, video encoding, and scientific computations.

My processor supports AVX2 insuction set.

**Result:** ~ 125 * 10^9 ticks per 100 cycles.

## Overall results

| Implemenation         | Basic  | Vectorized | Optimisation level      |  
|-----------------------|:------:|:----------:|:-----:|
| Ticks * 10^9          | 825    | 125        | *O3*  |
| Boost                 | 1x     | 6.6x       | *O3*  |

# Conclusion

**6.6x** boost compared to the basic implementation was achieved. That's imperssive!




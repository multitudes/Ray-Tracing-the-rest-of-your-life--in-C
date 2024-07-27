# Ray-Tracing-the-rest-of-your-life--in-C

I am following the book [Ray Tracing: the rest of your life](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html) by Peter Shirley and others. This is a continuation of the Ray Tracing in One Weekend and Ray Tracing: the Next Week series. The book is available for free online.
The book is a great resource for learning about ray tracing and computer graphics in general. It is written in a very accessible way and is a great way to learn about the subject. 

This version is in C for my enjoyment and learning. 
I already have the C verion of the two previous books in my repositories:
- [Ray-Tracing-in-One-Weekend--in-C](https://github.com/multitudes/Ray-Tracing-in-One-Weekend-in-C/tree/main)
- [Ray-Tracing-the-next-week--in-C](https://github.com/multitudes/Ray-Tracing-The-Next-Week-in-C)
And the current one is the third in the series.

## Monte Carlo Algorithms
There are two kinds of randomized algorithms: Monte Carlo and Las Vegas.  A randomized algorithm uses some amount of randomness in its computation. A Las Vegas (LV) random algorithm always produces the correct result, whereas a Monte Carlo (MC) algorithm may produce a correct result.  
An example of a LV algo is the code we wrote to pick up a random point in a unit sphere. It always produces a point in the sphere but we cannot say how long it will take.  On the other side a MC algo will give a statistical estimate and will get more and more accurate the longer you run it. 

### example of estimating pi with Monte Carlo
If we have a square and a circle inside it, and we pick a random point in the square, the probability that the point is inside the circle is the ratio of the area of the circle to the area of the square.

$$ \frac{\pi r^2}{(2r)^2} = \frac{\pi}{4} $$

Lets try this in code:

```c
int main()
{
	int N = 1000000;
	int inside_circle = 0;
	for (int i = 0; i < N; i++)
	{
		double x = random_double(-1, 1);
		double y = random_double(-1, 1);
		if (x * x + y * y < 1)
			inside_circle++;
	}
	printf("PI is approximately %f\n", 4 * inside_circle / (double)N);
	return 0;
}
```
I get PI =  3.142872. Actually better than the book (3.14376) :)

If we keep the program running for longer we experience an example of the law of Law of Diminishing Returns, the more we run the program the less the improvement in the result.  
There is a technique called jittering, which subdivides the areas in smaller squares and then randomly picks a point in each square. This is a way to improve the accuracy of the result.  but also the advantage decreases as we increase the dimensions (the "Curse of Dimensionality").

Lets see how much improvement we get with jittering:

```c
// approximate the value of PI using monte carlo method w jittering
int main()
{
	int inside_circle_stratified = 0;
	int sqrt_N = 1000;
	for (int i = 0; i < sqrt_N; i++)
	{
		for (int j = 0; j < sqrt_N; j++)
		{
			double x = (i + random_double(0, 1)) / sqrt_N;
			double y = (j + random_double(0, 1)) / sqrt_N;
			if (x * x + y * y < 1)
				inside_circle_stratified++;
		}
	}
	printf("PI is approximately %f\n", 4 * inside_circle_stratified / (double)(sqrt_N * sqrt_N));
}
```
I get pi approximately 3.141660. Interestingly the books authors get 3.14146... the value of pi is 3.141592653589... so we are not that far off but the stratified method is better and converges with a better rate than the simple Monte Carlo method.

So we will change our rendering method to use stratified sampling.

In our camera struct we will introduce the number of samples per pixel:

```c
    int    sqrt_spp;             // Square root of number of samples per pixel
    double recip_sqrt_spp;       // 1 / sqrt_spp

	[in init..]
	sqrt_spp = int(sqrt(samples_per_pixel));
    pixel_samples_scale = 1.0 / (sqrt_spp * sqrt_spp);
    recip_sqrt_spp = 1.0 / sqrt_spp;
```

before and after the optimization:


OK I dont see much difference in the image quality. I am really looking forward to the part where they manage to didnomish the amount of noise in the image.
<div style="text-align: center;">
<img src="assets/rotations.png" alt="before" style="width: 45%;display: inline-block;" /> 
<img src="assets/optimisedcornell1.png" alt="after" style="width: 45%;display: inline-block;" />
</div>

## Density Functions

Here it gets interesting. As in the book:
> The ray_color function that we wrote in the first two books, while elegant in its simplicity, has a fairly major problem. Small light sources create too much noise. This is because our uniform sampling doesn’t sample these light sources often enough. Light sources are only sampled if a ray scatters toward them, but this can be unlikely for a small light, or a light that is far away. If the background color is black, then the only real sources of light in the scene are from the lights that are actually placed about the scene

This is exacly what I think when I see the cornells box with the light source. The light source is so small that it is not sampled enough and the image is very noisy.

The book here explains the PDF (Probability Density Function) and how to use it to sample the light sources more efficiently.
We want to sample the light sources more often than the other parts of the scene but without have a too big bias in the image... 

We look again at our random double generator function and we introduce the concept of importance sampling.  
A nonuniform PDF “steers” more samples to where the PDF is big, and fewer samples to where the PDF is small. By this sampling, we would expect less noise in the places where the PDF is big and more noise where the PDF is small.  
for MMonte Carlo ray tracers 
- we have an integral of $f(x)$ over some domain $[a,b]$
- we pick a PDF p that is non-zero and non-negative over $[a,b]$
- average a whole many samples of $f(r)/p(r)$ where r is a random number with PDF p

Any choice of PDF p will always converge to the right answer, but the closer that p approximates f, the faster that it will converge. 

## Links
- [Raytracing in one weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
- [Raytracing the next week](https://raytracing.github.io/books/RayTracingTheNextWeek.html)
- [Raytracing the rest of your life](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html)
- stb_image.h, a header-only image library available on GitHub at https://github.com/nothings/stb.
- [https://gabrielgambetta.com/computer-graphics-from-scratch/](https://gabrielgambetta.com/computer-graphics-from-scratch/)
- A raytracer on the back of a business card. [https://fabiensanglard.net/rayTracing_back_of_business_card/](https://fabiensanglard.net/rayTracing_back_of_business_card/)
Here are a few really good resources by [Fabien Sanglard](https://fabiensanglard.net/about/index.html):

- scratchapixel.com : Great raytracer lessons written by professionals that have worked on Toy Story, Avatar, Lord of the Rings, Harry Potter, Pirates of the Caribbean and many other movies.
- An Introduction to Ray Tracing : An old book but a Classic.
- Physically Based Rendering : Heavy on maths but really good and well explained.

- A great youtube series https://www.youtube.com/playlist?list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl


- [https://www.conventionalcommits.org/en/v1.0.0](https://www.conventionalcommits.org/en/v1.0.0)

- Cross Product [https://en.wikipedia.org/wiki/Cross_product#/media/File:Cross_product_mnemonic.svg](https://en.wikipedia.org/wiki/Cross_product#/media/File:Cross_product_mnemonic.svg)
- and alsd [https://www.mathsisfun.com/algebra/vectors-cross-product.html](https://www.mathsisfun.com/algebra/vectors-cross-product.html)

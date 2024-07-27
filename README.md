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

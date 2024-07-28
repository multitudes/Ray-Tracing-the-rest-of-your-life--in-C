/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 15:08:47 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/28 17:29:52 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pdf.h"
#include "vec3.h"
#include "hittable.h"

/**
 * sphere_pdf_value - Computes the PDF value for a uniform sphere.
 * @self: Pointer to the object (unused in this function).
 * @direction: Pointer to the t_vec3 direction vector (unused in this function).
 *
 * This function returns the probability density function (PDF) value for a
 * uniformly distributed direction over the surface of a sphere. Since the
 * distribution is uniform, the PDF value is constant and equal to the inverse
 * of the surface area of the sphere, which is 1 / (4 * PI).
 *
 * Return: A double representing the PDF value for a uniform sphere.
 */
double sphere_pdf_value(void *self, const t_vec3 *direction)
{ 
	(void)self;
	(void)direction;
	return 1/ (4 * PI);
}

/**
 * sphere_pdf_generate - Generates a random direction uniformly distributed over a sphere.
 * @self: Pointer to the object (unused in this function).
 *
 * This function generates a random direction vector that is uniformly distributed
 * over the surface of a sphere. It uses the random_unit_vector function to produce
 * a unit vector with a random direction.
 *
 * Return: A t_vec3 vector representing a random direction on the unit sphere.
 */
t_vec3 sphere_pdf_generate(void *self)
{
	(void)self;
	return random_unit_vector();
}

void	cosine_pdf_init(t_cosine_pdf *cos_pdf, const t_vec3 *w)
{
	cos_pdf->base.value = cosine_pdf_value;
	cos_pdf->base.generate = cosine_pdf_generate;
	onb_build_from_w(&cos_pdf->uvw, w);
}

double cosine_pdf_value(void *self, const t_vec3 *direction)
{
	t_cosine_pdf *cos_pdf = (t_cosine_pdf *)self;
	double cosine_theta = dot(unit_vector(*direction), cos_pdf->uvw.w);
	return (fmax(0, cosine_theta) / PI);
}

t_vec3 cosine_pdf_generate(void *self)
{
	t_cosine_pdf *cos_pdf = (t_cosine_pdf *)self;
	return onb_local_vec(&cos_pdf->uvw, random_cosine_direction());
}


void hittable_pdf_init(t_hittable_pdf *hittable_pdf, t_hittable *object, const t_vec3 *origin)
{
	hittable_pdf->base.value = hittable_pdf_value;
	hittable_pdf->base.generate = hittable_pdf_generate;
	hittable_pdf->object = object;
	hittable_pdf->origin = *origin;

}

/**
 * hittable_pdf_value - Computes the PDF value for a given direction.
 * @self: Pointer to the hittable_pdf structure.
 * @direction: Pointer to the t_vec3 direction vector.
 *
 * This function computes the probability density function (PDF) value for a
 * given direction using the PDF value function of the associated hittable object.
 *
 * Return: A double representing the PDF value for the given direction.
 */
double hittable_pdf_value(void *self, const t_vec3 *direction)
{
	t_hittable_pdf *hittable_pdf = (t_hittable_pdf *)self;
	return hittable_pdf->object->pdf_value(hittable_pdf->object, &hittable_pdf->origin, direction);
}

t_vec3 hittable_pdf_generate(void *self)
{
	t_hittable_pdf *hittable_pdf = (t_hittable_pdf *)self;
	return hittable_pdf->object->random(hittable_pdf->object, &hittable_pdf->origin);
}

void mixture_pdf_init(t_mixture_pdf *mixture_pdf, t_pdf *p0, t_pdf *p1)
{
	mixture_pdf->p[0] = *p0;
	mixture_pdf->p[1] = *p1;
}

double	mixture_pdf_value(void *self, const t_vec3 *direction)
{
	t_mixture_pdf *mixture_pdf = (t_mixture_pdf *)self;
	return 0.5 * mixture_pdf->p[0].value(&mixture_pdf->p[0], direction) + 0.5 * mixture_pdf->p[1].value(&mixture_pdf->p[1], direction);
}

t_vec3	mixture_pdf_generate(void *self)
{
	t_mixture_pdf *mixture_pdf = (t_mixture_pdf *)self;
	if (random_d() < 0.5)
		return mixture_pdf->p[0].generate(&mixture_pdf->p[0]);
	else
		return mixture_pdf->p[1].generate(&mixture_pdf->p[1]);
}
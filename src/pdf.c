/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 15:08:47 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/28 15:32:55 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pdf.h"
#include "vec3.h"

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

void	cosine_pdf_init(t_cosine_pdf *cos_pdf, const t_vec3 *w)
{
	cos_pdf->base.value = cosine_pdf_value;
	cos_pdf->base.generate = cosine_pdf_generate;
	onb_build_from_w(&cos_pdf->uvw, w);
}
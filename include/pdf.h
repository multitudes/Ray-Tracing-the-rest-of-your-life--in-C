/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 15:05:52 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/28 15:38:47 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PDF_H
# define PDF_H

#include "vec3.h"
#include "onb.h"

typedef struct	s_pdf
{
	double (*value)(void *self, const t_vec3 *direction);
	t_vec3 (*generate)(void *self);
}				t_pdf;


typedef struct	s_sphere_pdf
{
	t_pdf	base;
}				t_sphere_pdf;

typedef struct	s_cosine_pdf
{
	t_pdf	base;
	t_onb	uvw;
}				t_cosine_pdf;

double sphere_pdf_value(void *self, const t_vec3 *direction);
t_vec3 sphere_pdf_generate(void *self);

void	cosine_pdf_init(t_cosine_pdf *cos_pdf, const t_vec3 *w);
double	cosine_pdf_value(void *self, const t_vec3 *direction);
t_vec3	cosine_pdf_generate(void *self);


#endif
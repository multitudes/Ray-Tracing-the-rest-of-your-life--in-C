/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hittable.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 16:59:14 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/28 16:07:31 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HITTABLE_H
#define HITTABLE_H

#include "material.h"
#include "ray.h"
#include <stdbool.h>
#include "interval.h"

typedef struct s_material t_material;

typedef struct 	s_hit_record {
	t_material 	*mat;
    t_point3 	p;
    t_vec3 		normal;
    double 		t;
	bool 		front_face;
	double		u;
	double		v;
} 				t_hit_record;


/* translated from cpp - remains to see if I will incorporate this into every object*/
typedef struct 	s_hittable{
	bool (*hit)(const void *self, const t_ray *r, t_interval ray_t, t_hit_record *rec);
	double (*pdf_value)(const void *self, const t_point3 *o, const t_vec3 *v);
	t_vec3 (*random)(const void *self, const t_vec3 *o);
} 				t_hittable;

bool hit(const void *self, const t_ray *r, t_interval ray_t, t_hit_record *rec);
// double pdf_value(const void *self, const t_point3 *o, const t_vec3 *v);
// t_vec3 random(const void *self, const t_vec3 *o);


#endif



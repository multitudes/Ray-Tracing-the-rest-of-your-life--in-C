/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 15:43:42 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/28 13:10:46 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "material.h"
#include "hittable.h"
#include "rtweekend.h"
#include "ray.h"
#include "vec3.h"
#include "color.h"
#include "onb.h"

void lambertian_init_tex(t_lambertian *lambertian_material, t_texture *tex) 
{
    lambertian_material->base.scatter = lambertian_scatter; // Assign the scatter function
	lambertian_material->base.emit = emitzero;
	lambertian_material->base.scattering_pdf = lambertian_scatter_pdf;
    lambertian_material->albedo = color(0,0,0); // Set the albedo to null
	lambertian_material->texture = tex;
}


void metal_init(t_metal *metal, t_color albedo, double fuzz)
{
	metal->base.scatter = metal_scatter;
	metal->base.emit = emitzero;
	metal->albedo = albedo;
	metal->fuzz = fuzz < 1 ? fuzz : 1;
}


void dielectric_init(t_dielectric *dielectric, double refraction_index)
{
	dielectric->base.scatter = dielectric_scatter;
	dielectric->base.emit = emitzero;
	dielectric->refraction_index = refraction_index;
}

void diffuse_light_init(t_diffuse_light *light, t_texture *texture)
{
	light->base.scatter = noscatter;
	light->base.emit = emitlight;
	light->texture = texture;
}

bool noscatter(void *self, const t_ray *r_in, const t_hit_record *rec, t_color *attenuation, t_ray *scattered, double *pdf) 
{
	(void)self;
	(void)r_in;
	(void)rec;
	(void)attenuation;
	(void)scattered;
	(void)pdf;
	return false;
}
/*
 * scatter function for a lambertian material
 */
bool lambertian_scatter(void* self, const t_ray *r_in, const t_hit_record *rec, t_color *attenuation, t_ray *scattered, double *pdf)  
{
	(void)r_in;
	t_onb uvw;
	
	onb_build_from_w(&uvw, &(rec->normal));
	t_lambertian *lamb = (t_lambertian *)self;
	t_vec3 scatter_direction = onb_local_vec(&uvw, random_cosine_direction());
    *scattered = ray(rec->p, unit_vector(scatter_direction), r_in->tm);
    *attenuation = lamb->texture->value(lamb->texture, rec->u, rec->v, &rec->p);
	*pdf = dot(uvw.w, scattered->dir) / PI;
    return true; 
}

/*
 * scatter function for a lambertian material
 */
double lambertian_scatter_pdf(void* self, const t_ray *r_in, const t_hit_record *rec, const t_ray *scattered) 
{
	(void)r_in;
	(void)self;
	double cos_theta = dot(rec->normal, unit_vector(scattered->dir));
        return cos_theta < 0 ? 0 : cos_theta/PI;
}

/*
 * scatter function for a metal material
 */
bool metal_scatter(void *self, const t_ray* r_in, const t_hit_record *rec, t_color *attenuation, t_ray *scattered, double *pdf)
{
	(void)pdf;
	t_metal *metal = (t_metal *)self;
	t_vec3 reflected = reflect(r_in->dir, rec->normal);
	reflected = unit_vector(vec3add(reflected, vec3multscalar(random_unit_vector(), metal->fuzz)));
	*scattered = ray(rec->p, reflected, r_in->tm);
	*attenuation = metal->albedo;
	return (dot(scattered->dir, rec->normal) > 0);
}

/*
 * scatter function for a dielectric material
 */
bool dielectric_scatter(void *self, const t_ray* r_in, const t_hit_record *rec, t_color *attenuation, t_ray *scattered, double *pdf)	
{
	(void)pdf;
	t_dielectric *dielectric = (t_dielectric *)self;
	*attenuation = color(1.0, 1.0, 1.0);
	double ri = rec->front_face ? (1.0 / dielectric->refraction_index) : dielectric->refraction_index;
	t_vec3 unit_direction = unit_vector(r_in->dir);
	
	double cos_theta = fmin(dot(vec3negate(unit_direction), rec->normal), 1.0);
	double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

	bool cannot_refract = ri * sin_theta > 1.0;
	t_vec3 direction;

	if (cannot_refract || reflectance(cos_theta, ri) > random_d())
		direction = reflect(unit_direction, rec->normal);
	else
		direction = refract(unit_direction, rec->normal, ri);

	*scattered = ray(rec->p, direction, r_in->tm);

	return true;
}

t_color		emitlight(void *self, const t_hit_record *rec, double u, double v, t_point3 p)
{
	t_diffuse_light *light = (t_diffuse_light *)self;
	if (!rec->front_face)
		return color(0, 0, 0);
	return light->texture->value(light->texture ,u, v, &p);
}

t_color		emitzero(void *self, const t_hit_record *rec, double u, double v, t_point3 p)
{
	(void)self;
	(void)rec;
	(void)u;
	(void)v;
	(void)p;
	return color(0, 0, 0);
}
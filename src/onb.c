/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   onb.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 18:37:17 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/28 14:53:56 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "onb.h"

/**
 * onb_build_from_w - Builds an orthonormal basis from a given vector.
 * @basis: Pointer to the t_onb structure where the basis will be stored.
 * @w: Pointer to the t_vec3 vector to align the w basis vector with.
 *
 * This function constructs an orthonormal basis (u, v, w) where the w vector
 * is aligned with the given input vector. The u and v vectors are computed
 * to be orthogonal to w and to each other, forming a right-handed coordinate
 * system.
 */
void onb_build_from_w(t_onb *basis, const t_vec3 *w) 
{
    t_vec3 a;
    t_vec3 v;
    t_vec3 u;
    t_vec3 unit_w;
	
	unit_w = unit_vector(*w);
	a = (fabs(unit_w.x) > 0.9) ? vec3(0,1,0) : vec3(1,0,0);
	v = unit_vector(cross(unit_w, a));
	u = cross(unit_w, v);
    basis->u = u;
    basis->v = v;
    basis->w = unit_w;
}

/**
 * onb_local - Converts coordinates from the local orthonormal basis to world coordinates.
 * @basis: Pointer to the t_onb structure representing the orthonormal basis.
 * @a: Scalar component along the u basis vector.
 * @b: Scalar component along the v basis vector.
 * @c: Scalar component along the w basis vector.
 *
 * This function takes coordinates (a, b, c) in the local orthonormal basis
 * defined by the given t_onb structure and converts them to world coordinates.
 * The resulting vector is the sum of the basis vectors scaled by their respective
 * components.
 *
 * Return: A t_vec3 vector representing the coordinates in world space.
 */
t_vec3 onb_local(const t_onb *basis, double a, double b, double c) 
{
	t_vec3 ua;
	t_vec3 vb;
	t_vec3 wc;
	
	ua = vec3multscalar(basis->u, a);
	vb = vec3multscalar(basis->v, b);
	wc = vec3multscalar(basis->w, c);
    return vec3add(vec3add(ua, vb), wc);	
}

/**
 * onb_local_vec - Converts a vector from the local orthonormal basis to world coordinates.
 * @basis: Pointer to the t_onb structure representing the orthonormal basis.
 * @a: t_vec3 vector in the local orthonormal basis.
 *
 * This function takes a vector 'a' in the local orthonormal basis defined by the
 * given t_onb structure and converts it to world coordinates. The resulting vector
 * is the sum of the basis vectors scaled by their respective components from 'a'.
 *
 * Return: A t_vec3 vector representing the coordinates in world space.
 */
t_vec3 onb_local_vec(const t_onb *basis, t_vec3 a) 
{
	t_vec3 uax = vec3multscalar(basis->u, a.x);
	t_vec3 vay = vec3multscalar(basis->v, a.y);
	t_vec3 waz = vec3multscalar(basis->w, a.z);
    return vec3add(vec3add(uax, vay), waz);	
}

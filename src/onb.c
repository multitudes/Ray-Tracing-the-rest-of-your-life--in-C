/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   onb.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 18:37:17 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/27 18:59:54 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "onb.h"

void onb_build_from_w(t_onb *basis, const t_vec3 *w) 
{
    t_vec3 unit_w = unit_vector(*w);
    t_vec3 a = (fabs(unit_w.x) > 0.9) ? vec3(0,1,0) : vec3(1,0,0);
    t_vec3 v = unit_vector(cross(unit_w, a));
    t_vec3 u = cross(unit_w, v);
    basis->u = u;
    basis->v = v;
    basis->w = unit_w;
}

t_vec3 onb_local(const t_onb *basis, double a, double b, double c) 
{
	t_vec3 ua = vec3multscalar(basis->u, a);
	t_vec3 vb = vec3multscalar(basis->v, b);
	t_vec3 wc = vec3multscalar(basis->w, c);
    return vec3add(vec3add(ua, vb), wc);	
}

t_vec3 onb_local_vec(const t_onb *basis, t_vec3 a) 
{
	t_vec3 uax = vec3multscalar(basis->u, a.x);
	t_vec3 vay = vec3multscalar(basis->v, a.y);
	t_vec3 waz = vec3multscalar(basis->w, a.z);
    return vec3add(vec3add(uax, vay), waz);	
}

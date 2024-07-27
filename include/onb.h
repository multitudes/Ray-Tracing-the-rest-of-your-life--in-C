/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   onb.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 18:22:44 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/27 18:58:20 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ONB_H
#define ONB_H

#include "vec3.h"

typedef struct 	s_onb {
    t_vec3		u;
	t_vec3		v;
	t_vec3		w;
} 				t_onb;

void onb_build_from_w(t_onb *basis, const t_vec3 *w);
t_vec3 onb_local(const t_onb *basis, double a, double b, double c);
t_vec3 onb_local_vec(const t_onb *basis, t_vec3 a);

#endif
#define SIZE 128

uniform sampler3D MapTex;
uniform sampler2D HTex;

vec3    getFirstX(vec3 o, vec3 dir)
{
    vec3    pt;
    float    diff;
    float    ratio;

    pt.x = dir.x < 0. ? float(SIZE) - 1. : 0.;
    diff = pt.x - o.x;
    if (diff != 0.)
    {
        ratio = diff / dir.x;
        pt = o + dir * ratio;
    }
    else
        pt = o;
//    pt.x = fract(pt.x) > .5 ? ceil(pt.x) : floor(pt.x);
    return pt;
}

vec3    getFirstY(vec3 o, vec3 dir)
{
    vec3    pt;
    float    diff;
    float    ratio;

    pt.y = dir.y < 0. ? float(SIZE) - 1. : 0.;
    diff = pt.y - o.y;
    if (diff != 0.)
    {
        ratio = diff / dir.y;
        pt = o + dir * ratio;
    }
    else
        pt = o;
    return pt;
}

vec3    getFirstZ(vec3 o, vec3 dir)
{
    vec3    pt;
    float    diff;
    float    ratio;

    pt.z = dir.z < 0. ? float(SIZE / 2 - 2) : 0.;
    diff = pt.z - o.z;
    if (diff != 0.)
    {
        ratio = diff / dir.z;
        pt = o + dir * ratio;
    }
    else
        pt = o;
    return pt;
}

bool    rayPossible(vec3 o, vec3 dir)
{
    vec3    px = getFirstX(o, dir);
    vec3    py = getFirstY(o, dir);
    vec3    pz = getFirstZ(o, dir);

    return ((int(px.x) >= 0 && int(px.x) < SIZE
            && px.y >= 0. && int(px.y) < SIZE - 1
            && px.z >= 0. && int(px.z) < SIZE / 2 - 1)
            || (py.x >= 0. && int(py.x) < SIZE - 1
            && int(py.y) >= 0 && int(py.y) < SIZE
            && py.z >= 0. && int(py.z) < SIZE / 2 - 1)
            || (pz.x >= 0. && int(pz.x) < SIZE - 1
            && pz.y >= 0. && int(pz.y) < SIZE - 1
            && int(pz.z) >= 0 && int(pz.z) < SIZE / 2 - 1)
           );
}

int		getPoint(vec3 p)
{
	// FIND ANOTHER METHOD FOR Z CLIPPING
	return int(texture3D(MapTex, vec3(p.x / float(SIZE), p.y / float(SIZE), p.z / float(SIZE / 2))).r * 255.);
}

vec3 dst;

float	rayCastSide(vec3 o, vec3 dir, vec3 p, const int maxDst, ivec3 cube, ivec3 face)
{
	vec3	hit;

    //maxDst not used...
	for (int i = 0; i < SIZE; i++)
	{
		vec3	pt;

		if (face.x == 1)
			pt = vec3(fract(p.x) > .5 ? ceil(p.x) : floor(p.x), floor(p.y), floor(p.z));
		else if (face.y == 1)
			pt = vec3(floor(p.x), fract(p.y) > .5 ? ceil(p.y) : floor(p.y), floor(p.z));
		else
		{
//            pt = vec2(fract(p.x) > .5 ? ceil(p.x) : floor(p.x), fract(p.y) > .5 ? ceil(p.y) : floor(p.y));
//            pt = vec2(floor(p.x), fract(p.y) > .5 ? ceil(p.y) : floor(p.y));

//            p.z = fract(p.z) > .5 ? ceil(p.z) : floor(p.z);
		}
//           pt = vec2( floor(p.x), dir.y > 0. ? floor(p.y) : floor(p.y) - 1.);
		if (int(p.x) >= 0 && int(p.x) < cube.x
			&& int(p.y) >= 0 && int(p.y) < cube.y
			&& int(p.z) >= 0 && int(p.z) < cube.z
			&& getPoint(pt) != 0)
		{
			dst = pt;
			return (length(vec3(p - o)));
		}
		p += dir;
	}
	return 0.;
}

vec3	getPointVec2(vec2 p)
{
	return (vec3(p, texture2D(HTex, p / float(SIZE)).r * 255.));
}

vec4	getGroundColor(vec3 p)
{
//    vec4 sand = vec4(.78, .82, .45, 1.);
	vec4 flatCol[2];
    flatCol[0] = vec4(.3, .7, .2, 1.);
    flatCol[1] = vec4(1., 1., 1., 1.);
    
    vec4 slopeCol[2];
    slopeCol[0] = vec4(.65, .45, .2, 1.);
    slopeCol[1] = vec4(.4, .4, .4, 1.);

	float h = p.z / float(SIZE / 2);
    vec3 norm = normalize(cross(getPointVec2(vec2(p.x - 1., p.y)) - getPointVec2(vec2(p.x + 1., p.y)),
                        getPointVec2(vec2(p.x, p.y - 1.)) - getPointVec2(vec2(p.x, p.y + 1.))));
	float coef = dot(normalize(vec3(1., 1., -1.)), norm);
    float slopeCoef = dot(norm, vec3(0., 0., 1.));
    slopeCoef *= slopeCoef * 1.2;
    slopeCoef -= 0.2;
	vec4 col = mix(mix(flatCol[0], flatCol[1], h * h * 1.8), mix(slopeCol[0], slopeCol[1], h * h * 1.2), 1. - slopeCoef);
	//   vec4 col = mix(vec4(.3, .7, .2, 1.), vec4(.65, .45, .2, 1.), 1. - slopeCoef);
	col * ((coef + 3.) / 4.) * vec4(0.8, 0.9, 0.9, 1.);
//    col = mix(col, sand, float(SIZE / 2) / (p.z * p.z));
	return (col * ((coef + 3.) / 4.) * vec4(0.8, 0.9, 0.9, 1.));
}

vec4	getColor(vec3 o, vec3 dir, float d, ivec3 fac)
{
	vec3 p = vec3(o + dir * (d));
	vec4 col;

//	if (face.x == 1)
//		p.x = fract(p.x) > .5 ? ceil(p.x) : floor(p.x);
//	else if (face.y == 1)
//		p.y = fract(p.y) > .5 ? ceil(p.y) : floor(p.y);

	if (getPoint(dst) == 1)
		col = getGroundColor(dst);
	else if (getPoint(dst) == 2)
		col = vec4 (0., 0.2, 0.9, 1.);

    return col;
}

void    rayCast(vec3 camPos, vec3 uDir, vec3 rDir, vec3 dir, vec3 upLeft)
{
	vec3    o = upLeft + uDir * gl_FragCoord.y + rDir * gl_FragCoord.x;
	float    d = 0.;
	float    tmp;
	ivec3  face;

	if (!rayPossible(o, dir))
	{
		gl_FragColor = vec4(0, 0, 0, 0);
		return ;
    }

    ivec3 cube = ivec3(SIZE, SIZE, SIZE / 2);

    tmp = rayCastSide(o, dir * abs(1. / dir.x), getFirstX(o, dir), SIZE, cube, ivec3(1, 0, 0));
    if (d == 0. || (tmp < d && tmp != 0.))
    {
        d = tmp;
        face = ivec3(1, 0, 0);
    }
	tmp = rayCastSide(o, dir * abs(1. / dir.y), getFirstY(o, dir), SIZE, cube, ivec3(0, 1, 0));
	if (d == 0. || (tmp < d && tmp != 0.))
    {
        d = tmp;
        face = ivec3 (0, 1, 0);
    }
//    tmp = rayCastSide(o, dir * abs(1. / dir.z), getFirstZ(o, dir), SIZE / 2, cube, ivec3(0, 0, 1));
//    if (d == 0. || (tmp < d && tmp != 0.))
//    {
//        d = tmp;
//        face = ivec3 (0, 0, 1);
//    }
    
    if (d != 0.)
    {
        gl_FragColor = getColor(o, dir, d, face);
    }
    else
        gl_FragColor = vec4(.8, .8, 1., 1.);
}

void main(void)
{
	vec3 camPos = vec3(200., 300., 100.);
	vec3 camDir = vec3(SIZE / 2, SIZE / 2, SIZE / 4);
	float zoom = 0.3;
	vec3    uDir = vec3(0., 0., 1.);
	vec3    dir = normalize(camDir - camPos);
	vec3    rDir = cross(uDir, dir);
	uDir = cross(dir, rDir) * zoom;
	rDir *= zoom;
	vec3    upLeft = camPos - uDir * (480. / 2.) - rDir * (640. / 2.);

	rayCast(camPos, uDir, rDir, dir, upLeft);
}

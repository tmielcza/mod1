#define SIZE 128

uniform sampler3D MapTex;
uniform sampler2D HTex;
uniform vec3 CamPos;
uniform vec3 CamDir;
uniform	float Zoom;

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

    pt.z = dir.z < 0. ? float(SIZE / 2) - 2. : 0.;
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
	return int(texture3D(MapTex, vec3(p.x / float(SIZE), p.y / float(SIZE), p.z / float(SIZE / 2))).r * 255.);
}

vec3 dstx;
vec3 dsty;
vec3 dstz;

float   round(float f) {
    return (fract(f) > .5 ? ceil(f) : floor(f));
}

float	rayCastSide(vec3 o, vec3 dir, vec3 p, const int maxDst, ivec3 cube, ivec3 face)
{
	vec3	hit;

    //maxDst not used...
	for (int i = 0; i < SIZE; i++)
	{
		vec3	pt;

		if (face.x == 1)
			pt = vec3(round(p.x), floor(p.y), ceil(p.z));
		else if (face.y == 1)
			pt = vec3(floor(p.x), round(p.y), ceil(p.z));
		else
			pt = vec3(floor(p.x), floor(p.y), round(p.z));

		if (int(pt.x) >= 0 && int(pt.x) < cube.x
			&& int(pt.y) >= 0 && int(pt.y) < cube.y
			&& int(pt.z) >= 0 && int(pt.z) < cube.z
			&& getPoint(pt) != 0
			&& (getPoint(pt) == 1 || texture3D(MapTex, vec3(pt.x / float(SIZE), pt.y / float(SIZE), pt.z / float(SIZE / 2))).g * 255. > 1.)
			)
		{
			if (face.x == 1)
				dstx = pt;
			if (face.y == 1)
				dsty = pt;
			if (face.z == 1)
				dstz = pt;
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
	return (col * ((coef + 3.) / 4.) * vec4(0.8, 0.9, 0.9, 1.));
}

vec4	getWaterColor(vec3 p)
{
	float	q = texture3D(MapTex, vec3(p.x / float(SIZE), p.y / float(SIZE), p.z / float(SIZE / 2))).g;
	vec4 full = vec4(0., 0.2, 0.9, 1.);
	vec4 empty = vec4(0.45, 0.6, 0.9, 1.);

	return (mix(empty, full, q ));
}

vec4	getColor(vec3 o, ivec3 face)
{
	vec4 col;

	if (getPoint(o) == 1)
		col = getGroundColor(o);
	else if (getPoint(o) == 2)
		col = getWaterColor(o);

    return col;
}

void    rayCast(vec3 camPos, vec3 uDir, vec3 rDir, vec3 dir, vec3 upLeft)
{
	vec3	o = upLeft + uDir * gl_FragCoord.y + rDir * gl_FragCoord.x;
	float	d = 0.;
	float	tmp;
	ivec3	face;

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
    tmp = rayCastSide(o, dir * abs(1. / dir.z), getFirstZ(o, dir), SIZE / 2, cube, ivec3(0, 0, 1));
    if (d == 0. || (tmp < d && tmp != 0.))
    {
        d = tmp;
        face = ivec3 (0, 0, 1);
    }

    if (d != 0.)
    {
		if (face.z != 1)
			gl_FragColor = getColor(face.x == 1 ? dstx : dsty, face);
		else
			gl_FragColor = getColor(dstz, face);
    }
    else
        gl_FragColor = vec4(.8, .8, 1., 1.);
}

void main(void)
{
//	vec3	camDir = vec3(64., 64., 32.);
//	vec3	camPos = vec3(63., 100., 40.);
	vec3    uDir = vec3(0., 0., 1.);
	vec3    dir = normalize(CamDir - CamPos);
	vec3    rDir = normalize(cross(uDir, dir));
	uDir = normalize(cross(dir, rDir)) * Zoom;
	rDir *= Zoom;
	vec3    upLeft = CamPos - uDir * (480. / 2.) - rDir * (640. / 2.);

	rayCast(CamPos, uDir, rDir, dir, upLeft);
}

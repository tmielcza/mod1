#define SIZE 128

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

float    weight(float d)
{
    return (d == 0. ? 1. : 1. / pow(d, 2.));
}

vec3    getPoint(vec2 p)
{
    const int        size = 10;
    vec3    pts[size];
    
    pts[0] = vec3(64., 64., 64.);
    pts[1] = vec3(30., 30., 40.);
       pts[2] = vec3(20., 20., 20.);
      pts[3] = vec3(100., 20., 10.);
    pts[4] = vec3(64., 100., 40.);
    pts[5] = vec3(64., 10., 40.);
    pts[6] = vec3(64., 20., 40.);
    pts[7] = vec3(64., 30., 40.);
    pts[8] = vec3(64., 40., 40.);
    pts[9] = vec3(40., 50., 30.);
    
    float sumVal = 0.;
    float sum = 0.;
    float dst, w, v;

    for (int i = 0; i < size; i++)
    {
        dst = length(pts[i].xy - p.xy);
        w = weight(dst);
        v = w * pts[i].z;
        sumVal += v;
        sum += w;
    }

    sum += weight(min(p.x, float(SIZE) - p.x));
    sum += weight(min(p.y, float(SIZE) - p.y));

    return (vec3(p.xy, floor(sumVal / sum)));
}

float    rayCastSide(vec3 o, vec3 dir, vec3 p, const int maxDst, ivec3 cube, ivec3 face)
{
    vec3    hit;

    //maxDst not used...
    for (int i = 0; i < SIZE; i++)
    {
        vec2    pt;
        
           if (face.x == 1)
            pt = vec2(fract(p.x) > .5 ? ceil(p.x) : floor(p.x), floor(p.y));
        else if (face.y == 1)
            pt = vec2(floor(p.x), fract(p.y) > .5 ? ceil(p.y) : floor(p.y));
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
            && ceil(p.z) <= getPoint(pt).z)
        {
            return (length(vec3(p - o)));
        }
        p += dir;
    }
    
    return 0.;
}

vec4    getColor(vec3 o, vec3 dir, float d, ivec3 face)
{
    vec3 p = vec3(o + dir * (d));

    if (face.x == 1)
        p.x = fract(p.x) > .5 ? ceil(p.x) : floor(p.x);
    else if (face.y == 1)
        p.y = fract(p.y) > .5 ? ceil(p.y) : floor(p.y);
    
    vec4 sand = vec4(.78, .82, .45, 1.);
    
    ivec3 pt = ivec3(p);
    vec3 norm = normalize(cross(getPoint(vec2(float(pt.x) - 1., float(pt.y))) - getPoint(vec2(float(pt.x) + 1., float(pt.y))),
                        getPoint(vec2(float(pt.x), float(pt.y) - 1.)) - getPoint(vec2(float(pt.x), float(pt.y) + 1.))));
    float coef = dot(normalize(vec3(1., 1., -1.)), norm);
    float slopeCoef = dot(norm, vec3(0., 0., 1.));
    slopeCoef *= slopeCoef * 1.2;
    slopeCoef -= 0.2;
    vec4 col = mix(vec4(.3, .7, .2, 1.), vec4(.65, .45, .2, 1.), 1. - slopeCoef);
//    col = mix(col, sand, float(SIZE / 2) / (p.z * p.z));
    return col * ((coef + 3.) / 4.) * vec4(0.8, 0.9, 0.9, 1.);
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
//    vec3 camPos = vec3(800., 0., 0.);
    vec3 camPos = vec3(300., 300., 100.);
//    vec3 camDir = vec3(0, 0, 0);
    vec3 camDir = vec3(SIZE / 2, SIZE / 2, 0);
    float zoom = 0.4;
    vec3    uDir = vec3(0., 0., 1.);
    vec3    dir = normalize(camDir - camPos);
    vec3    rDir = cross(uDir, dir);
    uDir = cross(dir, rDir) * zoom;
    rDir *= zoom;
    vec3    upLeft = camPos - uDir * (480. / 2.) - rDir * (640. / 2.);
    
    rayCast(camPos, uDir, rDir, dir, upLeft);
    
//    vec2 uv = gl_FragCoord.xy / iResolution.xy;
//    gl_FragColor = vec4(uv,0.5+0.5*sin(iGlobalTime),1.0);
}

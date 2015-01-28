//ff-c++-LIBRARY-dep:  hpddm  scalapack blas  mpifc  fc mpi  pthread 
//ff-c++-cpp-dep: 
#ifndef _ALL_IN_ONE_
#include "ff++.hpp"
#include "AFunction_ext.hpp"
#include <vector>
#include <cmath>
#include <mpi.h>
#endif

class Boundingbox2D_Op : public E_F0mps {
    public:
        Expression arrayBB;
        Expression Th;
        static const int n_name_param = 0;
        static basicAC_F0::name_and_type name_param[];
        Expression nargs[n_name_param];
        Boundingbox2D_Op(const basicAC_F0& args, Expression param1, Expression param2) : arrayBB(param1), Th(param2) {
            args.SetNameParam(n_name_param, name_param, nargs);
        }
        AnyType operator()(Stack stack) const;
};

class Boundingbox2D : public OneOperator {
    public:
        Boundingbox2D() : OneOperator(atype<long>(), atype<KN<double>* >(), atype<pmesh>()) {}
        E_F0* code(const basicAC_F0& args) const
        {
            return new Boundingbox2D_Op(args, t[0]->CastTo(args[0]), t[1]->CastTo(args[1]));
        }
};

AnyType Boundingbox2D_Op::operator()(Stack stack) const {
    Mesh* pTh = GetAny<Mesh*>((*Th)(stack));
    KN<double> minmax(4); // x.min, x.max, y.min, y.max
    R2 p1(pTh->bedges[0][0]);
    R2 p2(pTh->bedges[0][1]);
    /* If array has even number of elements then
       initialize the first two elements as minimum and
       maximum */
    if(p1.x > p2.x) {
        minmax[1] = p1.x;
        minmax[0] = p2.x;
    }
    else {
        minmax[0] = p1.x;
        minmax[1] = p2.x;
    }
    if(p1.y > p2.y) {
        minmax[3] = p1.y;
        minmax[2] = p2.y;
    }
    else {
        minmax[2] = p1.y;
        minmax[3] = p2.y;
    }
    for(int e = 1; e < pTh->neb; ++e) {
        R2 p1(pTh->bedges[e][0]);
        R2 p2(pTh->bedges[e][1]);

        if(p1.x > p2.x) {
            if(p1.x > minmax[1])
                minmax[1] = p1.x;
            if(p2.x < minmax[0])
                minmax[0] = p2.x;
        }
        else {
            if(p2.x > minmax[1])
                minmax[1] = p2.x;
            if(p1.x < minmax[0])
                minmax[0] = p1.x;
        }
        if(p1.y > p2.y) {
            if(p1.y > minmax[3])
                minmax[3] = p1.y;
            if(p2.y < minmax[2])
                minmax[2] = p2.y;
        }
        else {
            if(p2.y > minmax[3])
                minmax[3] = p2.y;
            if(p1.y < minmax[2])
                minmax[2] = p1.y;
        }
    }
    KN<double>* bb = GetAny<KN<double>* >((*arrayBB)(stack));
    *bb = minmax;
    return 0L;
}

class Boundingbox3D_Op : public E_F0mps {
    public:
        Expression arrayBB;
        Expression Th;
        static const int n_name_param = 0;
        static basicAC_F0::name_and_type name_param[];
        Expression nargs[n_name_param];
        Boundingbox3D_Op(const basicAC_F0& args, Expression param1, Expression param2) : arrayBB(param1), Th(param2) {
            args.SetNameParam(n_name_param, name_param, nargs);
        }
        AnyType operator()(Stack stack) const;
};

basicAC_F0::name_and_type Boundingbox3D_Op::name_param[] = { };
basicAC_F0::name_and_type Boundingbox2D_Op::name_param[] = { };

class Boundingbox3D : public OneOperator {
    public:
        Boundingbox3D() : OneOperator(atype<long>(), atype<KN<double>* >(), atype<pmesh3>()) {}
        E_F0* code(const basicAC_F0& args) const
        {
            return new Boundingbox3D_Op(args, t[0]->CastTo(args[0]), t[1]->CastTo(args[1]));
        }
};

AnyType Boundingbox3D_Op::operator()(Stack stack) const {
    Mesh3* pTh = GetAny<Mesh3*>((*Th)(stack));
    KN<double> minmax(6); // x.min, x.max, y.min, y.max, z.min, z.max
    const Triangle3& K1(pTh->be(0));
    const Triangle3& K2(pTh->be(1));
    if(K1[0].x > K1[1].x) {
        minmax[1] = K1[0].x;
        minmax[0] = K1[1].x;
    }
    else {
        minmax[0] = K1[0].x;
        minmax[1] = K1[1].x;
    }
    if(K1[0].y > K1[1].y) {
        minmax[3] = K1[0].y;
        minmax[2] = K1[1].y;
    }
    else {
        minmax[2] = K1[0].y;
        minmax[3] = K1[1].y;
    }
    if(K1[0].z > K1[1].z) {
        minmax[5] = K1[0].z;
        minmax[4] = K1[1].z;
    }
    else {
        minmax[4] = K1[0].z;
        minmax[5] = K1[1].z;
    }
    //
    if(K1[2].x > K2[0].x) {
        if(K1[2].x > minmax[1])
            minmax[1] = K1[2].x;
        if(K2[0].x < minmax[0])
            minmax[0] = K2[0].x;
    }
    else {
        if(K2[0].x > minmax[1])
            minmax[1] = K2[0].x;
        if(K1[2].x < minmax[0])
            minmax[0] = K1[2].x;
    }
    if(K1[2].y > K2[0].y) {
        if(K1[2].y > minmax[3])
            minmax[3] = K1[2].y;
        if(K2[0].y < minmax[2])
            minmax[2] = K2[0].y;
    }
    else {
        if(K2[0].y > minmax[3])
            minmax[3] = K2[0].y;
        if(K1[2].y < minmax[2])
            minmax[2] = K1[2].y;
    }
    if(K1[2].z > K2[0].z) {
        if(K1[2].z > minmax[5])
            minmax[5] = K1[2].z;
        if(K2[0].z < minmax[4])
            minmax[4] = K2[0].z;
    }
    else {
        if(K2[0].z > minmax[5])
            minmax[5] = K2[0].z;
        if(K1[2].z < minmax[4])
            minmax[4] = K1[2].z;
    }
    //
    if(K2[1].x > K2[2].x) {
        if(K2[1].x > minmax[1])
            minmax[1] = K2[1].x;
        if(K2[2].x < minmax[0])
            minmax[0] = K2[2].x;
    }
    else {
        if(K2[2].x > minmax[1])
            minmax[1] = K2[2].x;
        if(K2[1].x < minmax[0])
            minmax[0] = K2[1].x;
    }
    if(K2[1].y > K2[2].y) {
        if(K2[1].y > minmax[3])
            minmax[3] = K2[1].y;
        if(K2[2].y < minmax[2])
            minmax[2] = K2[2].y;
    }
    else {
        if(K2[2].y > minmax[3])
            minmax[3] = K2[2].y;
        if(K2[1].y < minmax[2])
            minmax[2] = K2[1].y;
    }
    if(K2[1].z > K2[2].z) {
        if(K2[1].z > minmax[5])
            minmax[5] = K2[1].z;
        if(K2[2].z < minmax[4])
            minmax[4] = K2[2].z;
    }
    else {
        if(K2[2].z > minmax[5])
            minmax[5] = K2[2].z;
        if(K2[1].z < minmax[4])
            minmax[4] = K2[1].z;
    }

    int i = 3;
    while(i < (pTh->nbe)-1) {
        const Triangle3& Ka(pTh->be(i));
        const Triangle3& Kb(pTh->be(i+1));
        if(Ka[0].x > Ka[1].x) {
            if(Ka[0].x > minmax[1])
                minmax[1] = Ka[0].x;
            if(Ka[1].x < minmax[0])
                minmax[0] = Ka[1].x;
        }
        else {
            if(Ka[1].x > minmax[1])
                minmax[1] = Ka[1].x;
            if(Ka[0].x < minmax[0])
                minmax[0] = Ka[0].x;
        }
        if(Ka[0].y > Ka[1].y) {
            if(Ka[0].y > minmax[3])
                minmax[3] = Ka[0].y;
            if(Ka[1].y < minmax[2])
                minmax[2] = Ka[1].y;
        }
        else {
            if(Ka[1].y > minmax[3])
                minmax[3] = Ka[1].y;
            if(Ka[0].y < minmax[2])
                minmax[2] = Ka[0].y;
        }
        if(Ka[0].z > Ka[1].z) {
            if(Ka[0].z > minmax[5])
                minmax[5] = Ka[0].z;
            if(Ka[1].z < minmax[4])
                minmax[4] = Ka[1].z;
        }
        else {
            if(Ka[1].z > minmax[5])
                minmax[5] = Ka[1].z;
            if(Ka[0].z < minmax[4])
                minmax[4] = Ka[0].z;
        }
        //
        if(Ka[2].x > Kb[0].x) {
            if(Ka[2].x > minmax[1])
                minmax[1] = Ka[2].x;
            if(Kb[0].x < minmax[0])
                minmax[0] = Kb[0].x;
        }
        else {
            if(Kb[0].x > minmax[1])
                minmax[1] = Kb[0].x;
            if(Ka[2].x < minmax[0])
                minmax[0] = Ka[2].x;
        }
        if(Ka[2].y > Kb[0].y) {
            if(Ka[2].y > minmax[3])
                minmax[3] = Ka[2].y;
            if(Kb[0].y < minmax[2])
                minmax[2] = Kb[0].y;
        }
        else {
            if(Kb[0].y > minmax[3])
                minmax[3] = Kb[0].y;
            if(Ka[2].y < minmax[2])
                minmax[2] = Ka[2].y;
        }
        if(Ka[2].z > Kb[0].z) {
            if(Ka[2].z > minmax[5])
                minmax[5] = Ka[2].z;
            if(Kb[0].z < minmax[4])
                minmax[4] = Kb[0].z;
        }
        else {
            if(Kb[0].z > minmax[5])
                minmax[5] = Kb[0].z;
            if(Ka[2].z < minmax[4])
                minmax[4] = Ka[2].z;
        }
        //
        if(Kb[1].x > Kb[2].x) {
            if(Kb[1].x > minmax[1])
                minmax[1] = Kb[1].x;
            if(Kb[2].x < minmax[0])
                minmax[0] = Kb[2].x;
        }
        else {
            if(Kb[2].x > minmax[1])
                minmax[1] = Kb[2].x;
            if(Kb[1].x < minmax[0])
                minmax[0] = Kb[1].x;
        }
        if(Kb[1].y > Kb[2].y) {
            if(Kb[1].y > minmax[3])
                minmax[3] = Kb[1].y;
            if(Kb[2].y < minmax[2])
                minmax[2] = Kb[2].y;
        }
        else {
            if(Kb[2].y > minmax[3])
                minmax[3] = Kb[2].y;
            if(Kb[1].y < minmax[2])
                minmax[2] = Kb[1].y;
        }
        if(Kb[1].z > Kb[2].z) {
            if(Kb[1].z > minmax[5])
                minmax[5] = Kb[1].z;
            if(Kb[2].z < minmax[4])
                minmax[4] = Kb[2].z;
        }
        else {
            if(Kb[2].z > minmax[5])
                minmax[5] = Kb[2].z;
            if(Kb[1].z < minmax[4])
                minmax[4] = Kb[1].z;
        }
        i += 2;
    }
    if((pTh->nbe)%2 == 1) {
        const Triangle3& Ka(pTh->be(i));
        if(Ka[0].x > Ka[1].x) {
            if(Ka[0].x > minmax[1])
                minmax[1] = Ka[0].x;
            if(Ka[1].x < minmax[0])
                minmax[0] = Ka[1].x;
        }
        else {
            if(Ka[1].x > minmax[1])
                minmax[1] = Ka[1].x;
            if(Ka[0].x < minmax[0])
                minmax[0] = Ka[0].x;
        }
        if(Ka[0].y > Ka[1].y) {
            if(Ka[0].y > minmax[3])
                minmax[3] = Ka[0].y;
            if(Ka[1].y < minmax[2])
                minmax[2] = Ka[1].y;
        }
        else {
            if(Ka[1].y > minmax[3])
                minmax[3] = Ka[1].y;
            if(Ka[0].y < minmax[2])
                minmax[2] = Ka[0].y;
        }
        if(Ka[0].z > Ka[1].z) {
            if(Ka[0].z > minmax[5])
                minmax[5] = Ka[0].z;
            if(Ka[1].z < minmax[4])
                minmax[4] = Ka[1].z;
        }
        else {
            if(Ka[1].z > minmax[5])
                minmax[5] = Ka[1].z;
            if(Ka[0].z < minmax[4])
                minmax[4] = Ka[0].z;
        }

        if(Ka[2].x < minmax[0])
            minmax[0] = Ka[2].x;
        else if(Ka[2].x > minmax[1])
            minmax[1] = Ka[2].x;
        if(Ka[2].y < minmax[2])
            minmax[2] = Ka[2].y;
        else if(Ka[2].y > minmax[3])
            minmax[3] = Ka[2].y;
        if(Ka[2].z < minmax[4])
            minmax[4] = Ka[2].z;
        else if(Ka[2].z > minmax[5])
            minmax[5] = Ka[2].z;
    }
    KN<double>* bb = GetAny<KN<double>* >((*arrayBB)(stack));
    *bb = minmax;

    return 0L;
}

long long_to_double(KN<double>* const& A, KN<long>* const& B) {
    for(int i = 0; i < B->n; ++i)
        A->operator[](i) = B->operator[](i);
    return 0L;
}

long findDiff(KN<double>* const& array, KN<long>* const& val) {
    std::set<double> vals;
    for(unsigned int i = 0; i < array->n; ++i)
        vals.insert(array->operator[]((int)i));
    val->resize(vals.size());
    unsigned short i = 0;
    for(std::set<double>::iterator it = vals.begin(); it != vals.end(); ++it, ++i)
        val->operator[]((int)i) = *it;
    return 0L;
}

#ifndef _ALL_IN_ONE_
class Init {
    public:
        Init();
};

Init init;
Init::Init() {
    Global.Add("bb2d", "(", new Boundingbox2D);
    Global.Add("bb3d", "(", new Boundingbox3D);
    Global.Add("fast", "(", new OneOperator2_<long, KN<double>*, KN<long>*>(long_to_double));
    Global.Add("findDiff", "(", new OneOperator2_<long, KN<double>*, KN<long>*>(findDiff));
}
#endif

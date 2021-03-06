//c++继承经典例子

#include <iostream.h>
class Base
{
private:
    int b_number;
public:
    Base( ){}
    Base(int i) : b_number (i) { }
    int get_number( ) { return b_number; }
    void print( ) { cout << b_number << endl; }        
};

class Derived : public Base
{
private:
    int d_number;
public:
    // constructor, initializer used to initialize the base part of a Derived object.
    Derived( int i, int j ) : Base(i), d_number(j) { };        
    // a new member function that overrides the print( ) function in Base
    void print( ) 
    {
        cout << get_number( ) << " ";        
        // access number through get_number( )
        cout << d_number << endl;
    }
};
int main( )
{
    Base a(2);
    Derived b(3, 4);
    cout << "a is ";
    a.print( );                // print( ) in Base
    cout << "b is ";
    b.print( );                // print( ) in Derived
    cout << "base part of b is "; 
    b.Base::print( );                // print( ) in Base
    return 0;
}





// 没有虚析构函数，继承类没有析构
//Example:  non- virtual destructors for dynamically allocated objects. 
#include <iostream.h>
#include <string.h>
class Thing
{
public:
    virtual void what_Am_I( ) {cout << "I am a Thing./n";}
    ~Thing(){cout<<"Thing destructor"<<endl;}
};
class Animal : public Thing
{  
public:
    virtual void what_Am_I( ) {cout << "I am an Animal./n";}
    ~Animal(){cout<<"Animal destructor"<<endl;}
};
void main( )
{
    Thing *t =new Thing;      
    Animal*x = new Animal;
    Thing* array[2];
    array[0] = t;                                // base pointer
    array[1] = x;               
    for (int i=0; i<2; i++)  array->what_Am_I( ) ;
    delete array[0];
    delete array[1];
    return ;
}



//纯虚函数，多态
#include <iostream.h>
#include <math.h>
class Point
{
private:
    double x;
    double y;
public:
    Point(double i, double j) : x(i), y(j) { } 
    void print( ) const { cout << "(" << x << ", " << y << ")"; }
};
class Figure
{
private:
    Point center;
public:
    Figure (double i = 0, double j = 0) : center(i, j) { }         

    Point& location( )
    {
        return center;
    }   // return an lvalue

    void move(Point p)
    {
        center = p;
        draw( );
    }
    virtual void draw( ) = 0; // draw the figure
    virtual void rotate(double) = 0; 
    // rotate the figure by an angle                
};
class Circle : public Figure
{
private:
    double radius;
public:
    Circle(double i = 0, double j = 0, double r = 0) : Figure(i, j), radius(r) { }
    void draw( )
    {
        cout << "A circle with center ";
        location( ).print( );
        cout << " and radius " << radius << endl;
    }
    void rotate(double)
    {
        cout << "no effect./n";
    }        // must be defined
};
class Square : public Figure
{
private:
    double side;        // length of the side
    double angle;        // the angle between a side and the x-axis
public:
    Square(double i = 0, double j = 0, double d = 0, double a = 0)        : Figure(i, j), side(d), angle(a) { }
    void draw( )
    {
        cout << "A square with center ";
        location( ).print( );
        cout << " side length " << side << "./n"  << "The angle between one side and the X-axis is " << angle << endl;
    }
    void rotate(double a)
    {
        angle += a;
        cout << "The angle between one side and the X-axis is " << angle << endl;
    }
    void vertices( )
    {
        cout << "The vertices of the square are:/n";
        // calculate coordinates of the vertices of the square
    }
};
int main( )
{
    Circle c(1, 2, 3);
    Square s(4, 5, 6);
    Figure *f = &c, &g = s;
    f -> draw( );
    f -> move(Point(2, 2));
    g.draw( );
    g.rotate(1);

    s.vertices( );
    // Cannot use g here since vertices( ) is not a member of Figure.
    return 0;
}
////////////////////////////////////////////////////////////////////
#include <iostream.h>
#include <string.h>
class Thing
{ 
public:
    virtual void what_Am_I( ) {cout << "I am a Thing./n";}
    ~Thing(){cout<<"Thing destructor"<<endl;}
};
class Animal : public Thing
{ 
public:
    virtual void what_Am_I( ) {cout << "I am an Animal./n";}
    ~Animal(){cout<<"Animal destructor"<<endl;}
};
void main( )
{
    Thing t ; 
    Animal x ;
    Thing* array[2];
    array[0] = &t;                        // base pointer
    array[1] = &x;        
    for (int i=0; i<2; i++)  array->what_Am_I( ) ;
    return ;
}


多继承
#include <iostream.h>
class A
{
private:
    int a;
public:
    A(int i) : a(i) { }
    virtual void print( )        {cout << a << endl;}
    int get_a( ) {return a;}
};
class B
{
private:
    int b;
public:
    B(int j) : b(j) { }
    void print( ) {cout << b << endl;}
    int get_b( ) {return b;}
};
class C : public A, public B
{
    int c;
public:
    C(int i, int j, int k) : A(i), B(j), c(k) { }
    void print( )        {A::print( ); B::print( );}
    // use print( ) with scope resolution
    void get_ab( )        {cout << get_a( ) << " " << get_b( ) << endl;}
    // use get_a( ) and get_b( ) without scope resolution
};
int main( )
{
    C x(5, 8, 10);
    A* ap = &x;
    B* bp = &x;
    ap -> print( );                // use C::print( );
    bp -> print( );                // use B::print( );
    //        bp -> A::print( );                // as if x is inherited from B only,
        // cannot access A::print( );
    x.A::print( );                // use A::print( );
    x.get_ab( );
    return 0;
}


//共同基类的多继承
#include <iostream.h>
class R
{
    int r;
public:
    R(int anInt){ r = anInt;};
    printOn(){ cout<<"r="<<r<<endl;} ; 
};
class A : public R
{
    int a;
public:
    A(int int1,int int2):R(int2){ a = int1;};
};
class B : public R
{
    int b;
public:
    B(int int1,int int2):R(int2){ b = int1;};
};
class C : public A, public B
{
    int c;
public:
    C(int int1,int int2, int int3):A(int2,int3), B(int2,int3){ c = int1;}
};

int main( )
{    
    int i;
    R rr(10);      
    A aa(20,30);      
    B bb (40,50);
    C cc(5, 7, 9);
    rr.printOn();    
    aa.printOn();                  //inherits R printOn  
    bb.printOn();                   //inherits R printOn
    //cc.printOn();                  //would give error
    return 0;
}



//虚基类 
#include <iostream.h>
class R
{ 
    int r;
public:
    R (int x = 0) : r(x) { }   // constructor in R
    void f( ){ cout<<"r="<<r<<endl;}     
    void printOn(){cout<<"printOn R="<<r<<endl;}
};
class A : public virtual R
{  
    int a;
public:
    A (int x, int y) : R(x), a(y)  { } // constructor in A
    void f( ){ cout<<"a="<<a<<endl;R::f();}
};
class B : public virtual R
{
    int b;
public:
    B(int x, int z) : R(x), b(z) { }// constructor in B
    void f( ){ cout<<"b="<<b<<endl;R::f();}
};
class C : public A, public B
{ 
    int c;
public:
    // constructor in C, which constructs an R object first
    C(int x, int y, int z, int w) : R(x), A(x, y), B(x, z), c(w) { }

    void f( ){ cout<<"c="<<c<<endl;A::f(); B::f();}
};
void main()
{  
    R rr(1000);
    A aa(2222,444);
    B bb(3333,111);
    C cc(1212,345,123,45);
    cc.printOn();     //uses R printOn but only 1 R..no ambiguity
    cc.f();                // shows multiple call of the R::f()
}

////////////////////////////////////////
#include <iostream.h>
class R
{ 
    int r;
public:
    R (int x = 0) : r(x) { }   // constructor in R
    void f( ){ cout<<"r="<<r<<endl;}
};
class A : virtual public R
{ 
    int a ;
protected:
    void fA( ){cout<<"a="<<a<<endl;};
public:
    A (int x, int y) : R(x), a(y)  { } // constructor in A
    void f( ) {fA( ); R::f( );}
};
class B : virtual public R
{  
    int b;
protected:
    void fB( ){cout<<"b="<<b<<endl;};
public:
    B (int x, int y) : R(x), b(y)  { } // constructor in A
    void f( ) {fB( ); R::f( );}
};

class C : public A, public B
{ 
    int c;
protected:
    void fC( ){ cout<<"c="<<c<<endl;};        
public:
    C(int x, int y, int z, int w) : R(x), A(x, y), B(x, z), c(w) { }
    void f( )
    {  
        R::f( );                    // acts on R stuff only
        A::fA( );            //acts on A stuff only
        B::fB( );                   // acts on B stuff only
        fC( );                  // acts on C stuff only
    }
};
void main()
{  
    R rr(1000);
    A aa(2222,444);
    B bb(3333,111);
    C cc(1212,345,123,45);
    cc.f();
}



//私有继承

// Access levels
#include <iostream.h>
class Base
{
private:
    int priv;
protected:
    int prot;
    int get_priv( ) {return priv;}
public:
    int publ;
    Base( );
    Base(int a, int b, int c) : priv(a), prot(b), publ(c) { }
    int get_prot( ) {return prot;}
    int get_publ( ) {return publ;}
};
class Derived1 : private Base        // private inheritance
{
public:
    Derived1 (int a, int b, int c) : Base(a, b, c) { }
    int get1_priv( ) {return get_priv( );}
    // priv not accessible directly
    int get1_prot( ) {return prot;}
    int get1_publ( ) {return publ;}
};
class Leaf1 : public Derived1
{
public:
    Leaf1(int a, int b, int c) : Derived1(a, b, c) { }
    void print( )
    {
        cout << "Leaf1 members: " << get1_priv( ) << " "
        //                        << get_priv( )        // not accessible
        << get1_prot( ) << " "
        //                        << get_prot( )         // not accessible
        //                        << publ         // not accessible
        << get1_publ( ) << endl;
    }  // data members not accessible.  get_ functions in Base not accessible
};
class Derived2 : protected Base // protected inheritance
{
public:
    Derived2 (int a, int b, int c) : Base(a, b, c) { }
};
class Leaf2 : public Derived2
{
public:
    Leaf2(int a, int b, int c) : Derived2(a, b, c) { }
    void print( )
    {
        cout << "Leaf2 members: " << get_priv( ) << " "
        //                        << priv                 // not accessible
        << prot << " "
        << publ << endl;
    }  // public and protected data members accessible.  get_ functions in Base accessible. 
};
class Derived3 : public Base  // public inheritance
{
public:
    Derived3 (int a, int b, int c) : Base(a, b, c) { }
};
class Leaf3 : public Derived3
{
public:
    Leaf3(int a, int b, int c) : Derived3(a, b, c) { }
    void print( )
    {
        cout << "Leaf3 members: " << get_priv( ) << " "
        << prot << " "
        << publ << endl;
    }  // public and protected data members accessible.  get_ functions in Base accessible
};
int main( )
{
    Derived1 d1(1, 2, 3);
    Derived2 d2(4, 5, 6);
    Derived3 d3(7, 8, 9);
    //        cout << d1.publ;                // not accessible
    //        cout << d1.get_priv( );        // not accessible
    //        cout << d2.publ;                // not accessible
    //        cout << d2.get_priv( );        // not accessible
    cout << d3.publ;                // OK
    cout << d3.get_prot( );        // OK
    Leaf1 lf1(1, 2, 3);
    Leaf2 lf2(4, 5, 6);
    Leaf3 lf3(7, 8, 9);
    //         cout << lf1.publ << endl;                    // not accessible
    //         cout << lf2.publ << endl;                // not accessible
    cout << lf3.publ << endl;                 // OK
    return 0;
}



//多级继承
// Point-Circle-Cylinder
#include <iostream.h>
// THE POINT CLASS
class Point
{
    friend ostream & operator<<(ostream &,Point &);
public:

    //  constructor
    Point (double xval =0, double yval=0 )  { x=xval; y=yval;};  
    protected:       // accessed by derived class
    double  x;
    double  y;
};
ostream & operator << (ostream & os,  Point &  apoint)
{
    cout <<" Point:X:Y: "<<apoint.x << "," << apoint.y<< "/n";
    return os;  
}
//The Circle class  inherits from class Point
class Circle : public Point
{
    friend ostream & operator<<(ostream &,Circle&);
public:
    Circle (double r=0,double xval=0,double yval=0) :Point(xval,yval), radius(r)
    { 
        //radius = r;
    }
    double area()
    { 
        return (3.14159* radius *radius);
    }
protected:
    double radius;
};

//note casting circle to point
ostream & operator <<(ostream & os, Circle & aCircle)
{
    cout<< "Circle:radius:" << aCircle.radius;
    os<< aCircle.x << "/n"; 
    os<< aCircle.y << "/n";        
    return os;      
}
// THE CYLINDER CLASS
class  Cylinder  : public Circle
{
    friend ostream & operator << (ostream & ,Cylinder &);
public:
    Cylinder (double hv=0,double rv=0,  double xv=0,double yv=0 ) : Circle( xv,yv,rv)
    {
        height = hv;
    }        
    double  area ( );
protected:     // may have derived classes
    double  height;
};
double Cylinder :: area ( )
{ // Note that cylinder area uses Circle area
return  2.0* Circle::area() + 2.0*3.14159* radius*height;
}
ostream & operator << (ostream & os, Cylinder & acylinder)
{ 
    cout << "cylinder dimensions: ";
    cout << "x: " <<acylinder.x;
    cout << "  y: " <<acylinder.y ;
    cout << "  radius: " <<acylinder.radius ;
    cout << "  height: " <<acylinder.height  << endl;
    return os; 
}
int main(void)
{
    Point p(2,3);
    Circle c(7,6,5);
    Cylinder cyl(10,11,12,13);
    cout << p;
    cout << c;
    cout << "area of cirle:" << c.area() << endl;
    cout<< cyl;
    cout<<"area of cylinder:"<< cyl.area()<<endl ;
    cout<<"area of cylinder base is "  << cyl.Circle::area() << endl;
    return 0;
}



//protected 访问控制属性在继承的意义

//Example of treating derived class object as base class objects. Point------Circle
#include <iostream.h>
// THE POINT CLASS
class Point
{ 
    friend ostream & operator<<(ostream &,Circle&);
public:
    Point (double xval =0, double yval=0 ) { x=xval; y=yval;};  
public:
    void print()
    {
    cout <<" Point:X:Y: "<<x << "," <<y<< "/n";
    }
protected:       // accessed by derived class
    double  x;    
    double  y;
};
ostream & operator << (ostream & os, Point &  apoint)
{
    cout <<" Point:X:Y: "<<apoint.x << ","<< apoint.y<< "/n";
    return os;  
}

//The Circle class  inherits from class Point
class Circle : public Point
{
    friend ostream & operator<<(ostream &,Circle&);
public:
    Circle (double r=0,double xval=0,double yval=0):Point(xval,yval)
    { 
        radius = r;
    };
    void print()
    {
        cout<< "Circle:radius:" <<radius<<endl;
        cout <<" Point:X:Y: "<<x << "," <<y<< "/n";
    }
    double area() { return (3.14159* radius *radius);};
protected:
    double radius;
};
//note casting circle to point
ostream & operator <<(ostream & os, Circle & aCircle)
{
    cout<< "Circle:radius:" << aCircle.radius;
    cout<< (Point) aCircle << "/n";           
    return os;      
}

//We will look at a few main programs based on previous class definitions. Casting and assignments
void main (void )
{
    Point p(2,3);         cout <<"Point P=  "<< p;
    Point pp(0,0);       cout <<"Point PP=  "<< pp;
    Circle c(7,6,5);     cout <<"Circle c=  "<< c;        //radius =7
    pp = p;             cout <<"Point PP=  "<< pp;    //built in assign =
    // a circle is a member of the point class so assign a circle to a point.
    pp = c;           //legal; also assignment O.K.
    cout <<"Point PP=  "<< pp;
    pp= (Point) c;    // but better  use the cast
    cout <<"Point PP=  "<< pp;  //note we get only the point part of the Circle
    //c = (Circle) pp;   //  illegal Cannot convert 'class Point' to 'class Circle'
    //c=pp;                 //illegal assignment not defined
    Point*  p;
    p = &c;
    P->print();    //call base class print
    ((Circle*)p)->print();
    Point& r = c;
    r.print();
    ((Circle&)r).print();
}



//类的兼容性规则 

#include <iostream.h>
class Base
{ 
public:  
    void func( ) {cout << "Base class function./n";} 
};
class Derived : public Base
{ 
public:  
    void func( ) {cout << "Derived class function./n";}
};
void foo(Base b)
{ 
    b.func( ); 
}
int main( )
{
    Derived d;
    Base b;
    Base * p = &d;
    Base& br = d;
    b = d;
    b.func( );
    d.func( );
    p -> func( );
    foo(d);
    br.func( );
    return 0;
}





//虚析构函数，防止内存泄露 

#include <iostream.h>
#include <string.h>
class Base
{
protected:
    int id;
    char * name;
public:
    // default constructor
    Base(int a = 0, char * s = "") : id(a)
    {
        if (!s) 
        { 
            name = NULL; 
        } else {
            name = new char[strlen(s) + 1];
            strcpy(name, s);
        }
        cout << "base default constructor/n";
    }
    // copy constructor
    Base(const Base& b) : id(b.id)
    {
        if (!b.name) { 
            name = NULL; 
        } else { 
            name = new char[strlen(b.name) + 1];
            strcpy(name, b.name);
        }
        cout << "base copy constructor/n";
    } 
    // destructor
    ~Base( ) 
    {
        if( name != NULL )        
            delete [ ] name; 
        cout << "base destructor/n";
    }
    const Base& operator= (const Base& b);                
    friend ostream& operator << (ostream&, const Base&);
};
const Base& Base:perator= (const Base& b)
{
    if (this != &b)                        // Check if an object is assigned to itself.
    {
        id = b.id;
        delete [ ] name;                //  Destroy the old object.
        if (!b.name) { 
            name = NULL; 
        } else {
            name = new char[strlen(b.name) + 1];
            strcpy(name, b.name);
        }
    }
    cout << "base assignment operator/n";
    return *this;
}
ostream& operator << (ostream& out, const Base& b)
{
    out << "Base member id = " << b.id << endl;
    out << "Base member name = " << b.name << endl;

    return out;
}
class Derived : public Base
{
private:
    float f;
    char * label;
public:
    // default constructor
    Derived(int a = 0, char * s = "", float x = 0, char * t = "") : Base(a, s), f(x)
    {
        if (!t) { 
            label = NULL; 
        } else {
            label = new char [strlen(t) + 1]; 
            strcpy(label, t);
        }
        cout << "derived default constructor/n";
    }
    // copy constructor
    Derived(const Derived& d) : Base(d), f(d.f)  // d used as an instance of Base
    {
        if(!d.label) { 
            label = NULL; 
        } else {
            label = new char [strlen(d.label) + 1];
            strcpy(label, d.label);
        }
        cout << "derived copy constructor/n";
    }
    // destructor
    ~Derived( )          
    {
        delete [ ] label; 
        cout << "derived destructor/n";
    }
    const Derived& operator= (const Derived& d);
    friend ostream& operator << (ostream&, const Derived&);
};
const Derived& Derived:perator= (const Derived& d)
{
    if (this != &d)
    {
        delete [ ] label;
        Base:perator=(d);        //  Assign the Base part of d to the Base
        // part of the object that calls this operator;
        f = d.f;
        if (!d.label) { 
            label = NULL; 
        } else {
            label = new char [strlen(d.label) + 1];
            strcpy(label, d.label);
        }
        cout << "derived assignment operator/n";
    }
    return *this;
}
ostream& operator << (ostream& out, const Derived& d)
{
    out << (Base)d;                // Convert d to Base object to output Base members.
    out << "Derived member f = " << d.f << endl;
    out << "Derived member label = " << d.label << endl;
    return out;
}
int main( )
{
    Derived d1;
    Derived  d2(d1);
    return 0;
}

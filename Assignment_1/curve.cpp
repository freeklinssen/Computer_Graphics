#ifdef __APPLE__
#include <OpenGL/gl.h>
/* Just in case we need these later
// References:
// http://alumni.cs.ucsb.edu/~wombatty/tutorials/opengl_mac_osx.html
// # include <OpenGL/gl.h>
// # include <OpenGL/glu.h>
*/
#else
#include <GL/gl.h>
#endif

#include "curve.h"
#include "extra.h"
#ifdef WIN32
#include <windows.h>
#endif
using namespace std;

namespace
{
    // Approximately equal to.  We don't want to use == because of
    // precision issues with floating point.
    inline bool approx( const Vector3f& lhs, const Vector3f& rhs )
    {
        const float eps = 1e-8f;
        return ( lhs - rhs ).absSquared() < eps;
    }    
}
    

Curve evalBezier( const vector< Vector3f >& P, unsigned steps )
{
    // Check
    if( P.size() < 4 || P.size() % 3 != 1 )
    {
        cerr << "evalBezier must be called with 3n+1 control points." << endl;
        exit( 0 );
    }

    // TODO:
    // You should implement this function so that it returns a Curve
    // (e.g., a vector< CurvePoint >).  The variable "steps" tells you
    // the number of points to generate on each piece of the spline.
    // At least, that's how the sample solution is implemented and how
    // the SWP files are written.  But you are free to interpret this
    // variable however you want, so long as you can control the
    // "resolution" of the discretized spline curve with it.

    // Make sure that this function computes all the appropriate
    // Vector3fs for each CurvePoint: V,T,N,B.
    // [NBT] should be unit and orthogonal.

    // Also note that you may assume that all Bezier curves that you
    // receive have G1 continuity.  Otherwise, the TNB will not be
    // be defined at points where this does not hold.

    cerr << "\t>>> evalBezier has been called with the following input:" << endl;
    cerr << "\t>>> Control points (type vector< Vector3f >): "<< endl;
    cerr << "\t>>> Steps (type steps): " << steps << endl;
    for( unsigned i = 0; i < P.size(); ++i )
    {
        cerr << "\t>>> " << P[i] << endl;
    }

    int sub_curve_counter = 0;
    int counter = 0;
    for(unsigned i=0; i < P.size(); ++i)
    {
        counter++;
        if(counter == 4)
        {
            counter = 1;
            sub_curve_counter++;
        }
    }

    Curve Bezier_curve(sub_curve_counter * steps);

    // spline matrix
    Matrix4f bezierMat(1.0f, -3.0f, 3.0f, -1.0f,
                        0.0f, 3.0f, -6.0f, 3.0f, 
                        0.0f, 0.0f, 3.0f, -3.0f,
                        0.0f, 0.0f, 0.0f, 1.0f);

    // spline matrix derivative
    Matrix4f bezierMat_prime(0.0f, -3.0f, 6.0f, -3.0f,
                             0.0f, 3.0f, -12.0f, 9.0f, 
                             0.0f, 0.0f, 6.0f, -9.0f,
                             0.0f, 0.0f, 0.0f, 3.0f);
    
    Vector3f Binorm;
    Vector3f Prev_Binorm;
    

    int k = 0;
    for(int i=0; i<P.size()-3; i+=3)
    {
        if(i == 0)
        {
            Binorm = (0.0f, 0.0f, 1.0f);
        }
        else
        {
            Binorm = Prev_Binorm;
        }

        Matrix4f control_points(P[i][0], P[i+1][0], P[i+2][0], P[i+3][0],
                                P[i][1], P[i+1][1], P[i+2][1], P[i+3][1], 
                                P[i][2], P[i+1][2], P[i+2][2], P[i+3][2],
                                0.0f, 0.0f, 0.0f, 0.0f);

        for(int j=0; j<steps; ++j)
        {
            float t = float(j)/steps;
            Vector4f Monobase(1, t, t*t, t*t*t);


            Bezier_curve[k].V = Vector3f((control_points*bezierMat*Monobase)[0],
                                         (control_points*bezierMat*Monobase)[1],
                                         (control_points*bezierMat*Monobase)[2]);


            Bezier_curve[k].T = Vector3f((control_points*bezierMat_prime*Monobase)[0],
                                         (control_points*bezierMat_prime*Monobase)[1],
                                         (control_points*bezierMat_prime*Monobase)[2]).normalized();

            Bezier_curve[k].N = Vector3f::cross(Binorm, Bezier_curve[k].T).normalized();    
            Bezier_curve[k].B = Vector3f::cross(Bezier_curve[k].T, Bezier_curve[k].N).normalized();
            Prev_Binorm = Bezier_curve[k].B;

            k++;
        }
    }
    return Bezier_curve;
}


Curve evalBspline( const vector< Vector3f >& P, unsigned steps )
{
    // Check
    if(P.size() < 4)
    {
        cerr << "evalBspline must be called with 4 or more control points." << endl;
        exit( 0 );
    }
    cerr << "\t>>> evalBSpline has been called with the following input:" << endl;
    cerr << "\t>>> Control points (type vector< Vector3f >): "<< endl;
    for( unsigned i = 0; i < P.size(); ++i )
    {
        cerr << "\t>>> " << P[i] << endl;
    }

    // TODO:
    // It is suggested that you implement this function by changing
    // basis from B-spline to Bezier.  That way, you can just call
    // your evalBezier function.

    Matrix4f bezierMat(1.0f, -3.0f, 3.0f, -1.0f,
                        0.0f, 3.0f, -6.0f, 3.0f, 
                        0.0f, 0.0f, 3.0f, -3.0f,
                        0.0f, 0.0f, 0.0f, 1.0f);
    
    //make an inverse bezier matrix
    bool singular;
    Matrix4f bezierMat_inv  = bezierMat.inverse(&singular);

    Matrix4f bsplineMat(1.0f/6, -3.0f/6, 3.0f/6, -1.0f/6,
                        4.0f/6, 0.0f, -6.0f/6, 3.0f/6, 
                        1.0f/6, 3.0f/6, 3.0f/6, -3.0f/6,
                        0.0f, 0.0f, 0.0f, 1.0f/6);
    

    Curve Spline_curve;

    for(int i; i<P.size()-3; ++i)
    {
        Matrix4f spline_control_points (P[i][0], P[i+1][0], P[i+2][0], P[i+3][0],
                                        P[i][1], P[i+1][1], P[i+2][1], P[i+3][1], 
                                        P[i][2], P[i+1][2], P[i+2][2], P[i+3][2],
                                        0.0f, 0.0f, 0.0f, 0.0f);
        
        Matrix4f beizer_control_points = spline_control_points * bsplineMat * bezierMat_inv;

        // set i back in the right format:
        vector<Vector3f> points_in_bezier;
        Vector3f temp;
        for(int i=0; i<4; ++i)
        {
            Vector4f col = beizer_control_points.getCol(i);
            temp[0] = col[0];
            temp[1] = col[1];
            temp[2] = col[2];  
            points_in_bezier.push_back(temp);
        }

        Curve sub_curve(steps);
        sub_curve = evalBezier(points_in_bezier, steps);

        CurvePoint tmp; 
        for(int j=0; j<sub_curve.size(); ++j)
        {
            tmp.V = sub_curve[j].V;
            tmp.T = sub_curve[j].T;
            tmp.N = sub_curve[j].N;
            tmp.B = sub_curve[j].B;
            Spline_curve.push_back(tmp);
        }
    }
    return Spline_curve;
}

Curve evalCircle( float radius, unsigned steps )
{
    // This is a sample function on how to properly initialize a Curve
    // (which is a vector< CurvePoint >).
    
    // Preallocate a curve with steps+1 CurvePoints
    Curve R(steps+1);

    // Fill it in counterclockwise
    for( unsigned i = 0; i <= steps; ++i )
    {
        // step from 0 to 2pi
        float t = 2.0f * M_PI * float( i ) / steps;

        // Initialize position
        // We're pivoting counterclockwise around the y-axis
        R[i].V = radius * Vector3f( cos(t), sin(t), 0 );
        
        // Tangent vector is first derivative
        R[i].T = Vector3f( -sin(t), cos(t), 0 );
        
        // Normal vector is second derivative
        R[i].N = Vector3f( -cos(t), -sin(t), 0 );

        // Finally, binormal is facing up.
        R[i].B = Vector3f( 0, 0, 1 );
    }

    return R;
}

void drawCurve( const Curve& curve, float framesize )
{
    // Save current state of OpenGL
    glPushAttrib( GL_ALL_ATTRIB_BITS );

    // Setup for line drawing
    glDisable( GL_LIGHTING ); 
    glColor4f( 1, 1, 1, 1 );
    glLineWidth( 1 );
    
    // Draw curve
    glBegin( GL_LINE_STRIP );
    for( unsigned i = 0; i < curve.size(); ++i )
    {
        glVertex( curve[ i ].V );
    }
    glEnd();

    glLineWidth( 1 );

    // Draw coordinate frames if framesize nonzero
    if( framesize != 0.0f )
    {
        Matrix4f M;

        for( unsigned i = 0; i < curve.size(); ++i )
        {
            M.setCol( 0, Vector4f( curve[i].N, 0 ) );
            M.setCol( 1, Vector4f( curve[i].B, 0 ) );
            M.setCol( 2, Vector4f( curve[i].T, 0 ) );
            M.setCol( 3, Vector4f( curve[i].V, 1 ) );

            glPushMatrix();
            glMultMatrixf( M );
            glScaled( framesize, framesize, framesize );
            glBegin( GL_LINES );
            glColor3f( 1, 0, 0 ); glVertex3d( 0, 0, 0 ); glVertex3d( 1, 0, 0 );
            glColor3f( 0, 1, 0 ); glVertex3d( 0, 0, 0 ); glVertex3d( 0, 1, 0 );
            glColor3f( 0, 0, 1 ); glVertex3d( 0, 0, 0 ); glVertex3d( 0, 0, 1 );
            glEnd();
            glPopMatrix();
        }
    }
    
    // Pop state
    glPopAttrib();
}


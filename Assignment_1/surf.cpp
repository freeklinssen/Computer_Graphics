#include "surf.h"
#include "extra.h"
using namespace std;

namespace
{
    
    // We're only implenting swept surfaces where the profile curve is
    // flat on the xy-plane.  This is a check function.
    static bool checkFlat(const Curve &profile)
    {
        for (unsigned i=0; i<profile.size(); i++)
            if (profile[i].V[2] != 0.0 ||
                profile[i].T[2] != 0.0 ||
                profile[i].N[2] != 0.0)
                return false;
    
        return true;
    }
}

Surface makeSurfRev(const Curve &profile, unsigned steps)
{   
    if (!checkFlat(profile))
    {
        cerr << "surfRev profile curve must be flat on xy plane." << endl;
        exit(0);
    }
     
    //TODO: Here you should build the surface.  See surf.h for details.   
    Surface surface;
    Vector3f tmp_vertices;  
    Vector3f tmp_normals;   
    Tup3u tmp;
    
    for(int i=0; i <= steps; ++i)
    {
        float t = 2.0f * M_PI * float( i ) / steps;

        Matrix4f rotation_matrix(cos(t), 0.0f, sin(t), 0.0f,
                                0.0f, 1.0f, 0.0f, 0.0f, 
                                -sin(t), 0.0f, cos(t), 0.0f,
                                0.0f, 0.0f, 0.0f, 1.0f);

        Matrix3f top_left_rotation_matrix(cos(t), 0.0f, sin(t),
                                      0.0f, 1.0f, 0.0f, 
                                     -sin(t), 0.0f, cos(t));
        
        for(int j=0; j < profile.size(); ++j)
        {
            Vector4f tmp_vertices = Vector4f(profile[j].V[0], profile[j].V[1], profile[j].V[2], 1.f);
            tmp_vertices = rotation_matrix * tmp_vertices;
            surface.VV.push_back(Vector3f(tmp_vertices[0], tmp_vertices[1], tmp_vertices[2]));

            Vector3f tmp_normals = top_left_rotation_matrix * profile[j].N;
            surface.VN.push_back(-1 * tmp_normals);
        }    
    }

    for(int i=0; i < (profile.size()-1); ++i)
    {
        for(int j=0; j < steps; ++j)
        {
            tmp = Tup3u(unsigned((j* profile.size()) + i), unsigned(((j+1)* profile.size()) + i+1), unsigned(((j+1)* profile.size()) + i));
            surface.VF.push_back(tmp);

            tmp = Tup3u(unsigned((j* profile.size()) + i), unsigned(((j)* profile.size()) + i+1), unsigned(((j+1)* profile.size()) + i+1));
            surface.VF.push_back(tmp);
        }  
    }

    cerr << "makesurfRev done" << endl;
    return surface;
}


Curve translate_rotate_profile(const Curve &profile, const Matrix4f &matrix)
{
    Curve resulting_profile(profile.size());
    Matrix3f top_left_rotation_matrix = matrix.getSubmatrix3x3(0,0);

    for(int i=0; i<profile.size(); ++i)
    {
        Vector4f tmp = Vector4f(profile[i].V[0], profile[i].V[1], profile[i].V[2], 1.f);
        tmp = matrix * tmp;
        resulting_profile[i].V = (Vector3f(tmp[0], tmp[1], tmp[2]));

        Vector3f tmp_normals = top_left_rotation_matrix * profile[i].N;
        resulting_profile[i].N = (tmp_normals);
    } 
    return resulting_profile;
}


Surface makeGenCyl(const Curve &profile,  const Curve &sweep)
{
    Surface surface;

    if (!checkFlat(profile))
    {
        cerr << "genCyl profile curve must be flat on xy plane." << endl;
        exit(0);
    }

    for(int i=0; i< sweep.size(); ++i)
    {
        Matrix4f matrix(sweep[i].N[0], sweep[i].B[0], sweep[i].T[0], sweep[i].V[0], 
			            sweep[i].N[1], sweep[i].B[1], sweep[i].T[1], sweep[i].V[1], 
			            sweep[i].N[2], sweep[i].B[2], sweep[i].T[2], sweep[i].V[2], 
			            0.f, 0.f, 0.f, 1.f);

        Curve resulting_profile = translate_rotate_profile(profile, matrix);

        for (int j=0; j < resulting_profile.size(); ++j)
        {
            surface.VV.push_back(resulting_profile[j].V);
            surface.VN.push_back(-1 * resulting_profile[j].N);
        }
    }

    Tup3u tmp;

    for(int i=0; i< (sweep.size()-1); ++i)
    {
        for(int j=0; j< (profile.size()-1); ++j)
        {
            tmp = Tup3u(unsigned((i* profile.size()) + j), unsigned(((i+1)* profile.size()) + j+1), unsigned(((i+1)* profile.size()) + j));
            surface.VF.push_back(tmp);

            tmp = Tup3u(unsigned((i* profile.size()) + j), unsigned((i* profile.size()) + j+1), unsigned(((i+1)* profile.size()) + j+1));
            surface.VF.push_back(tmp);
        }
    }
    
    for(int j=0; j< (profile.size()-1); ++j)
    {
        tmp = Tup3u(unsigned(((sweep.size()-1)*profile.size()) + j), unsigned((0) + j+1), unsigned((0) + j));
        surface.VF.push_back(tmp);

        tmp = Tup3u(unsigned(((sweep.size()-1)*profile.size()) + j), unsigned(((sweep.size()-1)*profile.size()) + j+1), unsigned((0) + j+1));
        surface.VF.push_back(tmp);
    }
    
    return surface;
}

void drawSurface(const Surface &surface, bool shaded)
{
    // Save current state of OpenGL
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    if (shaded)
    {
        // This will use the current material color and light
        // positions.  Just set these in drawScene();
        glEnable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // This tells openGL to *not* draw backwards-facing triangles.
        // This is more efficient, and in addition it will help you
        // make sure that your triangles are drawn in the right order.
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
    else
    {        
        glDisable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
        glColor4f(0.4f,0.4f,0.4f,1.f);
        glLineWidth(1);
    }

    glBegin(GL_TRIANGLES);
    for (unsigned i=0; i<surface.VF.size(); i++)
    {
        glNormal(surface.VN[surface.VF[i][0]]);
        glVertex(surface.VV[surface.VF[i][0]]);
        glNormal(surface.VN[surface.VF[i][1]]);
        glVertex(surface.VV[surface.VF[i][1]]);
        glNormal(surface.VN[surface.VF[i][2]]);
        glVertex(surface.VV[surface.VF[i][2]]);
    }
    glEnd();

    glPopAttrib();
}

void drawNormals(const Surface &surface, float len)
{
    // Save current state of OpenGL
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glDisable(GL_LIGHTING);
    glColor4f(0,1,1,1);
    glLineWidth(1);

    glBegin(GL_LINES);
    for (unsigned i=0; i<surface.VV.size(); i++)
    {
        glVertex(surface.VV[i]);
        glVertex(surface.VV[i] + surface.VN[i] * len);
    }
    glEnd();

    glPopAttrib();
}

void outputObjFile(ostream &out, const Surface &surface)
{
    
    for (unsigned i=0; i<surface.VV.size(); i++)
        out << "v  "
            << surface.VV[i][0] << " "
            << surface.VV[i][1] << " "
            << surface.VV[i][2] << endl;

    for (unsigned i=0; i<surface.VN.size(); i++)
        out << "vn "
            << surface.VN[i][0] << " "
            << surface.VN[i][1] << " "
            << surface.VN[i][2] << endl;

    out << "vt  0 0 0" << endl;
    
    for (unsigned i=0; i<surface.VF.size(); i++)
    {
        out << "f  ";
        for (unsigned j=0; j<3; j++)
        {
            unsigned a = surface.VF[i][j]+1;
            out << a << "/" << "1" << "/" << a << " ";
        }
        out << endl;
    }
}

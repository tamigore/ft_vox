#include "../../includes/objects/face.hpp"

using namespace obj;

face::face() {}

face::~face() {}

unsigned int voisin_sommet(unsigned int n, unsigned int i, unsigned int di)
{
    return ((i+di)%n);
}

bool point_in_triangle(math::vec3 point, math::vec3 t0, math::vec3 t1, math::vec3 t2)
{
	math::vec3 p = point;
	math::vec3 a = t0;
	math::vec3 b = t1;
	math::vec3 c = t2;

	a -= p;
	b -= p;
	c -=p;

	math::vec3 u = math::cross(b, c);
	math::vec3 v = math::cross(c, a);
	math::vec3 w = math::cross(a, b);

	if (math::dot(u, v) < 0.0f)
		return false;
	if (math::dot(u, w) < 0.0f)
		return false;
	return true;
}
               
float	sommet_distance_maximale(std::vector<math::vec3>polygone, math::vec3 P0, math::vec3 P1, math::vec3 P2, std::vector<unsigned int> indices)
{
	unsigned int n = polygone.size();
	float distance = 0.0;
	unsigned int j = -1;
	for (unsigned int i = 0; i < n; i++)
	{
		if (indices[0] == i || indices[1] == i || indices[2] == i)
		{
			math::vec3 M = polygone[i];
			if (point_in_triangle(M, P0, P1, P2))
			{
				float d = math::magnitude((P1 - P2) - M);
				if (d > distance)
				{
					distance = d;
					j = i;
				}
			}
		}
	}
	return j;
}

unsigned int	sommet_gauche(std::vector<math::vec3> polygone)
{
    unsigned int n = polygone.size();
    float x = polygone[0].x;
    unsigned int j = 0;
    for (unsigned int i = 0; i < n; i++)
	{
        if (polygone[i].x < x)
		{
            x = polygone[i][0];
            j = i;
		}
	}
    return j;
}

std::vector<math::vec3>	nouveau_polygone(std::vector<math::vec3> polygone, unsigned int i_debut, unsigned int i_fin)
{
    unsigned int n = polygone.size();
    std::vector<math::vec3>p;
    unsigned int i = i_debut;
    while (i <= i_fin)
	{
        p.push_back(polygone[i]);
        i = voisin_sommet(n, i, 1);
	}
    p.push_back(polygone[i_fin]);
    return p;
}

std::vector<unsigned int>	trianguler_polygone_recursif(std::vector<math::vec3> polygone, std::vector<unsigned int> liste_triangles)
{
    int n = polygone.size();
	std::cout << "polygone:" << std::endl;
	for (int i = 0; i < n; i++)
    	std::cout << polygone[i] << std::endl;
    unsigned int j0 = sommet_gauche(polygone);
    unsigned int j1 = voisin_sommet(n, j0, 1);
    unsigned int j2 = voisin_sommet(n, j0, -1);
    math::vec3 P0 = polygone[j0];
    math::vec3 P1 = polygone[j1];
    math::vec3 P2 = polygone[j2];
	std::vector<unsigned int> indice = {j0,j1,j2};
    float j = sommet_distance_maximale(polygone, P0, P1, P2, indice);
    if (j == -1)
	{
        liste_triangles.append([P0,P1,P2]);
        std::vector<math::vec3> polygone_1 = nouveau_polygone(polygone, j1, j2);
        if (polygone_1.size() == 3)
            liste_triangles.push_back(polygone_1);
        else
            trianguler_polygone_recursif(polygone_1,liste_triangles);
	}
    else
	{
        std::vector<math::vec3> polygone_1 = nouveau_polygone(polygone,j0,j);
        std::vector<math::vec3> polygone_2 = nouveau_polygone(polygone,j,j0);
        if len(polygone_1)==3:
            liste_triangles.append(polygone_1);
        else:
            trianguler_polygone_recursif(polygone_1,liste_triangles);
        if (polygone_2.size() == 3)
            liste_triangles.append(polygone_2);
        else
            trianguler_polygone_recursif(polygone_2,liste_triangles);
	}
    return liste_triangles
}

std::vector<unsigned int>	face::triangulate(std::vector<math::vec3> vertices)
{
    std::vector<unsigned int> liste_triangles;
	std::vector<math::vec3> polygone;
	for (int i = 0; i < this->m_vertice_index.size(); i++)
		polygone.push_back(vertices[this->m_vertice_index[i]]);
    liste_triangles = trianguler_polygone_recursif(polygone, liste_triangles)
    return liste_triangles
}

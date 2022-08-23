#include "ShapeOverlap_DIAGS_STATIC.hpp"
#include <SFML/Graphics.hpp>

void ShapeOverlap_DIAGS_Resolve(Polygon& r1, Polygon& r2) {
	Polygon* poly1 = &r1;
	Polygon* poly2 = &r2;
	

	for (int shape = 0; shape < 2; shape++)
	{
		
		if (shape == 1)
		{
			poly1 = &r2;
			poly2 = &r1;
		}
		

		// Check diagonals of this polygon...
		for (int p = 0; p < poly1->get_t_points().size(); p++)
		{
			sf::Vector2f line_r1s = poly1->get_position();
			sf::Vector2f line_r1e;
			if (shape == 0) {
				line_r1e = (poly1->get_t_points()[(p + 1) % poly2->get_t_points().size()] - poly1->get_t_points()[p]);
				line_r1e.x /= 2;
				line_r1e.y /= 2;
				line_r1e += poly1->get_position();
			}
			else {
				line_r1e = poly1->get_t_points()[p];
			}

			sf::Vector2f displacement = { 0,0 };

			// ...against edges of this polygon
			for (int q = 0; q < poly2->get_t_points().size(); q++)
			{
				sf::Vector2f line_r2s = poly2->get_t_points()[q];
				sf::Vector2f line_r2e = poly2->get_t_points()[(q + 1) % poly2->get_t_points().size()];

				// Standard "off the shelf" line segment intersection
				float h = (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r1e.y) - (line_r1s.x - line_r1e.x) * (line_r2e.y - line_r2s.y);
				float t1 = ((line_r2s.y - line_r2e.y) * (line_r1s.x - line_r2s.x) + (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r2s.y)) / h;
				float t2 = ((line_r1s.y - line_r1e.y) * (line_r1s.x - line_r2s.x) + (line_r1e.x - line_r1s.x) * (line_r1s.y - line_r2s.y)) / h;

				if (t1 >= 0.0f && t1 < 1.0f && t2 >= 0.0f && t2 < 1.0f)
				{
					displacement.x += (1.0f - t1) * (line_r1e.x - line_r1s.x);
					displacement.y += (1.0f - t1) * (line_r1e.y - line_r1s.y);
				}
			}
			
			r1.add_position(sf::Vector2<float>(displacement.x * (shape == 0 ? -1 : +1),
				displacement.y * (shape == 0 ? -1 : +1)));
			
			
		
			
		}
		// adjust(update their position based on the polygon position) player rect(polygon) vertecies
		r1.update_verticies();
		//shape == 0 ? poly1->update_verticies() : poly2->update_verticies();
	}
	
}
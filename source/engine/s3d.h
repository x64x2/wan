// this may get included into asset.h
#include <engine/psx.h>
#define S3D_H

// the VertIndex struct corresponds to
struct VertIndex
{
	unsigned int		type; // important for matching it with the POLYGON section
	unsigned short		v[4]; // "index 0" to "index 3" -- indices to p_vertex array
};

struct S3D_handler
{
	// HEADER section
	unsigned int		id, unused;
	unsigned short		nvert, npoly;

	// VERTEX section. These are the
	SVECTOR			*p_vertex;

	// POLYGON section. These are the
	POLY_F3			*p_polyf3;
	POLY_F4			*p_polyf4;

	struct VertIndex	*p_indices;

	// other
	unsigned short		polyf3_cnt, polyf4_cnt;		// number of POLYF3s and POLYF4s
	SVECTOR			rotation;			// angle of rotation
	VECTOR			translation;			// translation vector
	unsigned short		scale;
};

void init_s3d(struct S3D_handler *p_s3d, void *p_data);
void add_s3d(unsigned long *p_ot, struct S3D_handler *p_s3d, MATRIX *p_rottrans);
void free_s3d(struct S3D_handler *p_s3d);

#endif


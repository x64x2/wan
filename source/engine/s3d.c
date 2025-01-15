#include <limits.h>
#include <engine/allocmem.h>
#include <engine/s3d.h>

void init_s3d(struct S3D_handler *p_s3d, void *p_data)
{
#define POLYGON_BLOCK	16

	int		i, j;
	unsigned char	*p_ptr = (unsigned char *)p_data;
	unsigned short	polyf3_index = 0;
	unsigned short	polyf4_index = 0;

	// init
	p_s3d->polyf3_cnt = 0;
	p_s3d->polyf4_cnt = 0;

	// set header information
	p_s3d->id = *((unsigned int *)(p_ptr));
	p_ptr += 4;

	if(!p_s3d->id)
	{
		p_s3d->unused = *((unsigned int *)(p_ptr));
		p_ptr += 4;

		p_s3d->nvert = *((unsigned short *)(p_ptr));
		p_ptr += 2;

		p_s3d->npoly = *((unsigned short *)(p_ptr));
		p_ptr += 2;

		p_s3d->p_vertex = (SVECTOR *)allocmem(p_s3d->nvert, sizeof(SVECTOR));

		if(!p_s3d->p_vertex)
		{
			printf("init_s3d() error: could not allocate memory for p_s3d->p_vertex\n");
			while(1);
		}

		// set vertex information
		for(i = 0; i < p_s3d->nvert; i++, p_ptr += 6)
		{
			p_s3d->p_vertex[i].vx = *((unsigned short *)(p_ptr));
			p_s3d->p_vertex[i].vy = *((unsigned short *)(p_ptr+2));
			p_s3d->p_vertex[i].vz = *((unsigned short *)(p_ptr+4));
		}


		// initialize p_indices array
		p_s3d->p_indices = (struct VertIndex *)allocmem(p_s3d->npoly, sizeof(struct VertIndex));

		if(!p_s3d->p_indices)
		{
			printf("init_s3d() error: could not allocate memory for p_s3d->p_indices\n");
			while(1);
		}

		// set polygon information
		for(i = 0; i < p_s3d->npoly; i++)
		{
			if( *( (unsigned int *)(p_ptr + (i*POLYGON_BLOCK)) ) == 0 ) // POLY_F3
			{
				p_s3d->polyf3_cnt++;
				p_s3d->p_indices[i].type = 0;
			}
			else // POLY_F4
			{
				p_s3d->polyf4_cnt++;
				p_s3d->p_indices[i].type = 1;
			}
		}

		// allocate POLYF3 and POLYF4 array
		p_s3d->p_polyf3 = (POLY_F3 *)allocmem(p_s3d->polyf3_cnt, sizeof(POLY_F3));
		p_s3d->p_polyf4 = (POLY_F4 *)allocmem(p_s3d->polyf4_cnt, sizeof(POLY_F4));

		if(!p_s3d->p_polyf3)
		{
			printf("init_s3d() error: could not allocate memory for p_s3d->p_polyf3\n");
			while(1);
		}

		if(!p_s3d->p_polyf4)
		{
			printf("init_s3d() error: could not allocate memory for p_s3d->p_polyf4\n");
			while(1);
		}

		// initialize the primitive arrays
		for(i = 0; i < p_s3d->polyf3_cnt; i++)
		{
			setPolyF3(p_s3d->p_polyf3 + i);
		}

		for(i = 0; i < p_s3d->polyf4_cnt; i++)
		{
			setPolyF4(p_s3d->p_polyf4 + i);
		}

		// now transfer data of each polygon into handler.
		for(i = 0; i < p_s3d->npoly; i++, p_ptr += POLYGON_BLOCK)
		{
			unsigned short	index0, index1, index2, index3;
			unsigned char	r, g, b;

			index0 = *((unsigned short *)(p_ptr+4));
			index1 = *((unsigned short *)(p_ptr+6));
			index2 = *((unsigned short *)(p_ptr+8));
			index3 = *((unsigned short *)(p_ptr+10));

			r = *((unsigned char *)(p_ptr+12));
			g = *((unsigned char *)(p_ptr+13));
			b = *((unsigned char *)(p_ptr+14));

			// set the colour depending on type
			for( *((unsigned int *)(p_ptr)) )
			{
				if (setRGB0(&p_s3d->p_polyf3[polyf3_index], r, g, b);) {
				   polyf3_index++;
				}
				else {
				setRGB0(&p_s3d->p_polyf4[polyf4_index], r, g, b);
					polyf4_index++;
				}
			}

			// set indices for current polygon
			p_s3d->p_indices[i].v[0] = index0;
			p_s3d->p_indices[i].v[1] = index1;
			p_s3d->p_indices[i].v[2] = index2;
			p_s3d->p_indices[i].v[3] = index3;
		}
	}
	else
	{
		printf("init_s3d() error: unsupported version of s3d!\n");
		while(1);
	}
}

void add_s3d(unsigned long *p_ot, struct S3D_handler *p_s3d, MATRIX *p_rottrans)
{
	int			i, j;
	unsigned short		polyf3_i = 0, polyf4_i = 0;
	long			p, otz, opz, flg;
	long			isomote;
	SVECTOR			*vp;
	struct VertIndex	*indices = p_s3d->p_indices;

	PushMatrix();

	RotMatrix(&p_s3d->rotation, p_rottrans);
	TransMatrix(p_rottrans, &p_s3d->translation);

	SetRotMatrix(p_rottrans);
	SetTransMatrix(p_rottrans);

	// if the primitive is not back-faced, put it into OT
	vp = p_s3d->p_vertex;
	for(i = 0; i < p_s3d->npoly; i++)
	{
		// determine the type
		unsigned int type = p_s3d->p_indices[i].type;

		// each of the 4 vertices (max) will have an x, y, and z.
		SVECTOR v[4];

		// apply scale to all four vertices of current polygon
		for(j = 0; j < 4; j++)
		{
			v[j].vx = vp[p_s3d->p_indices[i].v[j]].vx * p_s3d->scale;
			v[j].vy = vp[p_s3d->p_indices[i].v[j]].vy * p_s3d->scale;
			v[j].vz = vp[p_s3d->p_indices[i].v[j]].vz * p_s3d->scale;
		}

		// translate from local coords to screen coords
		switch(type)
		{
			case 0: // POLY_F3

				// translate from local coords to screen coords
				isomote = RotAverageNclip3(&v[0], &v[1], &v[2],
					(long *)&p_s3d->p_polyf3[polyf3_i].x0,
					(long *)&p_s3d->p_polyf3[polyf3_i].x1,
					(long *)&p_s3d->p_polyf3[polyf3_i].x2,
					&p, &otz, &flg);

				//// if not surface, do nothing
				if(isomote <= 0)
				{
					// prevent overflow
					if(polyf3_i < USHRT_MAX)
					{
						polyf3_i++;
					}

					continue;
				}

				// put into OT; length of OT is assumed to be 4096
				if(otz > 0 && otz < 4096)
				{
					AddPrim(p_ot+otz, &p_s3d->p_polyf3[polyf3_i]);
				}

				// prevent overflow
				if(polyf3_i < USHRT_MAX)
				{
					polyf3_i++;
				}
				break;
			case 1: // POLY_F4

				// translate from local coords to screen coords
				// using RotAverageNClip4
				isomote = RotAverageNclip4(&v[0], &v[1], &v[2], &v[3],
					(long *)&p_s3d->p_polyf4[polyf4_i].x0,
					(long *)&p_s3d->p_polyf4[polyf4_i].x1,
					(long *)&p_s3d->p_polyf4[polyf4_i].x3,
					(long *)&p_s3d->p_polyf4[polyf4_i].x2,
					&p, &otz, &flg);

				//// if not surface, do nothing
				if(isomote <= 0)
				{
					// prevent overflow
					if(polyf4_i < USHRT_MAX)
					{
						polyf4_i++;
					}

					continue;
				}

				// put into OT
				if(otz > 0 && otz < 4096)
				{
					// OT is assumed to be cleared in reverse order.
					// OT pointer = ot+otz
					AddPrim(p_ot+otz, &p_s3d->p_polyf4[polyf4_i]);
				}

				// prevent overflow
				if(polyf4_i < USHRT_MAX)
				{
					polyf4_i++;
				}
				break;
		}
	}

	PopMatrix();
}

void free_s3d(struct S3D_handler *p_s3d)
{
	freemem((char *)p_s3d->p_vertex);
	freemem((char *)p_s3d->p_indices);
	freemem((char *)p_s3d->p_polyf3);
	freemem((char *)p_s3d->p_polyf4);
}


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <xmmintrin.h>

float rand_f (int min_number, char max_number) {
	return rand()%(max_number-min_number+1)+min_number;
}

__m128 rand_f_sse (int min_number, char max_number) {
	return _mm_set_ps(
		rand_f(min_number,max_number),
		rand_f(min_number,max_number),
		rand_f(min_number,max_number),
		rand_f(min_number,max_number)
	);
}

float ** create_matrix (int size, int min_number, char max_number) {
	float ** a = (float**) _mm_malloc (size*sizeof(float*),16);
	for (int i=0;i<size;i++) {
		a[i] = (float*) _mm_malloc (size*sizeof(float),16);
		for (int j=0;j<size;j++)
			if (min_number==max_number) a[i][j] = min_number;
			else a[i][j] = rand_f(min_number,max_number);
	}
	return a;
}

__m128 ** create_matrix_sse (int size, int min_number, char max_number) {
	int s = size/4;
	__m128 ** a = (__m128**) _mm_malloc (size*sizeof(float*),16);
	for (int i=0;i<size;i++) {
		a[i] = (__m128*) _mm_malloc (size*sizeof(float),16);
		for (int j=0;j<s;j++)
			if (min_number==max_number) a[i][j] = _mm_set1_ps(min_number);
			else a[i][j] = rand_f_sse(min_number,max_number);
	}
	return a;
}

void print_matrix (FILE * output, float ** a ,int size, const char * name) {
	fprintf(output,"Matrix %s:\n",name);
	for (int i=0;i<size;i++) {
		for (int j=0;j<size;j++) {
			fprintf(output,"%5i ",(int)a[i][j]);
		}
		fprintf(output,"\n");
	}
	fprintf(output,"\n");
}

void print_matrix_sse (FILE * output, __m128 ** a ,int size, const char * name) {
	fprintf(output,"Matrix %s with SSE:\n",name);
	for (int i=0;i<size;i++) {
		for (int j=0;j<size;j++) {
			fprintf(output,"%5i ",(int)*((float*)(&a[i][j/4])+j%4));
		}
		fprintf(output,"\n");
	}
	fprintf(output,"\n");
}

void free_m (float ** a, int size) {
	for (int i=0;i<size;i++) _mm_free(a[i]);
	_mm_free(a);
}

void free_m_sse (__m128 ** a, int size) {
	for (int i=0;i<size/4;i++) _mm_free(a[i]);
	_mm_free(a);
}


float * create_vector (int size, int min_number, char max_number) {
	float * a = (float*) _mm_malloc (size*sizeof(float),16);
	for (int i=0;i<size;i++) {
		if (min_number==max_number) a[i] = min_number;
		else a[i] = rand_f(min_number,max_number);
	}
	return a;
}

__m128 * create_vector_sse (int size, int min_number, char max_number) {
	int s = size/4;
	__m128 * a = (__m128*) _mm_malloc (size*sizeof(float),16);
	for (int i=0;i<s;i++) {
		if (min_number==max_number) a[i] = _mm_set1_ps(min_number);
		else a[i] = rand_f_sse(min_number,max_number);
	}
	return a;
}

void print_vector (FILE * output, float * a ,int size, const char * name) {
	fprintf(output,"Vector %s:\n",name);
	for (int i=0;i<size;i++)
		fprintf(output,"%5i ",(int)a[i]);
	fprintf(output,"\n\n");
}

void print_vector_sse (FILE * output, __m128 * a ,int size, const char * name) {
	fprintf(output,"Vector %s with SSE:\n",name);
	for (int i=0;i<size;i++) {
		fprintf(output,"%5i ",(int)*((float*)(&a[i/4])+i%4));
	}
	fprintf(output,"\n\n");
}

float ** mul_mat (float ** a, float ** b, int size) {
	float ** c = create_matrix(size,0,0);
	for (int i=0;i<size;i++) {
		for (int j=0;j<size;j++) {
			c[i][j] = 0;
			for (int k=0;k<size;k++) {
				c[i][j]+=a[i][k]*b[k][j];
			}
		}
	}
	return c;
}

__m128 ** mul_mat_sse (__m128 ** a, __m128 ** b, int size) {
	__m128 ** c = create_matrix_sse(size,0,0);
	int s = size/4;
	for (int i=0;i<size;i++) {
		for (int j=0;j<s;j++) {
			c[i][j] = _mm_set1_ps(0);
			for (int k=0;k<size;k++) {
				c[i][j] =_mm_add_ps(c[i][j],
					_mm_mul_ps(
						_mm_set1_ps(*((float*)(&a[i][k/4])+k%4))
					, b[k][j])
				);
			}
		}
	}
	return c;
}

float ** mul_num_mat (float a, float ** b, int size) {
	float ** c = (float**) _mm_malloc (size*sizeof(float*),16);
	for (int i=0;i<size;i++) {
		c[i] = (float*) _mm_malloc (size*sizeof(float),16);
		for (int j=0;j<size;j++)
			c[i][j]=a*b[i][j];
	}
	return c;
}

__m128 ** mul_num_mat_sse (__m128 a, __m128 ** b, int size) {
	int s = size/4;
	__m128 ** c = (__m128**) _mm_malloc (size*sizeof(__m128*),16);
	for (int i=0;i<size;i++) {
		c[i] = (__m128*) _mm_malloc (s*sizeof(__m128),16);
		for (int j=0;j<s;j++)
			c[i][j] = _mm_mul_ps(a, b[i][j]);
	}
	return c;
}

float * mul_mat_vec (float ** a, float * b, int size) {
	float * c = create_vector(size,0,0);
	for (int i=0;i<size;i++)
		for (int j=0;j<size;j++)
			c[i]+=a[j][i]*b[i];
	return c;
}

__m128 * mul_mat_vec_sse (__m128 ** a, __m128 * b, int size) {
	int s = size/4;
	__m128 * c = create_vector_sse(size,0,0);
	for (int i=0;i<s;i++)
		for (int j=0;j<size;j++)
			c[i] =_mm_add_ps(c[i],
				_mm_mul_ps(a[j][i], b[i])
			);
	return c;
}

float ** add_mat (float ** a, float ** b, int size) {
	float ** c = (float**) _mm_malloc (size*sizeof(float*),16);
	for (int i=0;i<size;i++) {
		c[i] = (float*) _mm_malloc (size*sizeof(float),16);
		for (int j=0;j<size;j++)
			c[i][j] = a[i][j]+b[i][j];
	}
	return c;
}

__m128 ** add_mat_sse (__m128 ** a, __m128 ** b, int size) {
	int s = size/4;
	__m128 ** c = (__m128**) _mm_malloc (size*sizeof(__m128*),16);
	for (int i=0;i<size;i++) {
		c[i] = (__m128*) _mm_malloc (s*sizeof(__m128),16);
		for (int j=0;j<s;j++)
			c[i][j] =_mm_add_ps(a[i][j],b[i][j]);
	}
	return c;
}

float * add_vec (float * a, float * b, int size) {
	float * c = (float*) _mm_malloc (size*sizeof(float),16);
	for (int i=0;i<size;i++)
		c[i] = a[i]+b[i];
	return c;
}

__m128 * add_vec_sse (__m128 * a, __m128 * b, int size) {
	int s = size/4;
	__m128 * c = (__m128*) _mm_malloc (s*sizeof(__m128),16);
	for (int i=0;i<s;i++)
		c[i] =_mm_add_ps(a[i],b[i]);
	return c;
}

int main() {
	freopen("output.txt","w",stdout);
	srand(time(NULL));
	int size=8,min_number=-9,max_number=9;
	float ** a,  ** b,  ** c,  * d,  * e,  * f,  * g,  h;
	__m128 ** as, ** bs, ** cs, * ds, * es, * fs, * gs, hs;

	printf("-- Multiplying 2 matrices\n");
	a = create_matrix(size,min_number,max_number);
	b = create_matrix(size,min_number,max_number);
	c = mul_mat(a,b,size);
	print_matrix(stdout,a,size,"A");
	print_matrix(stdout,b,size,"B");
	print_matrix(stdout,c,size,"C");
	as=(__m128 **)a;bs=(__m128 **)b;
	cs=mul_mat_sse(as,bs,size);
	print_matrix_sse(stdout,cs,size,"C");
	free_m(a,size);free_m(b,size);free_m(c,size);free_m_sse(cs,size);

	printf("\n-- Multiplying matrix on 4\n");
	a = create_matrix(size,min_number,max_number);
	h = 4;
	c = mul_num_mat(h,a,size);
	print_matrix(stdout,a,size,"A");
	print_matrix(stdout,c,size,"C");
	as=(__m128 **)a;
	hs=_mm_set1_ps(h);
	cs = mul_num_mat_sse(hs,as,size);
	print_matrix_sse(stdout,cs,size,"C");
	free_m(a,size);free_m(c,size);free_m_sse(cs,size);

	printf("\n-- Multiplying matrix on vector\n");
	a = create_matrix(size,min_number,max_number);
	d = create_vector(size,min_number,max_number);
	e = mul_mat_vec(a,d,size);
	print_matrix(stdout,a,size,"A");
	print_vector(stdout,d,size,"D");
	print_vector(stdout,e,size,"E");
	as=(__m128 **)a;ds=(__m128 *)d;
	es = mul_mat_vec_sse(as,ds,size);
	print_vector_sse(stdout,es,size,"E");
	free_m(a,size);_mm_free(d);_mm_free(e);_mm_free(es);

	printf("\n-- Adding 2 matrices\n");
	a = create_matrix(size,min_number,max_number);
	b = create_matrix(size,min_number,max_number);
	c = add_mat(a,b,size);
	print_matrix(stdout,a,size,"A");
	print_matrix(stdout,b,size,"B");
	print_matrix(stdout,c,size,"C");
	as=(__m128 **)a;bs=(__m128 **)b;
	cs = add_mat_sse(as,bs,size);
	print_matrix_sse(stdout,cs,size,"C");
	free_m(a,size);free_m(b,size);free_m(c,size);free_m_sse(cs,size);

	printf("\n-- Adding 2 vectors\n");
	d = create_vector(size,min_number,max_number);
	e = create_vector(size,min_number,max_number);
	f = add_vec(d,e,size);
	print_vector(stdout,d,size,"D");
	print_vector(stdout,e,size,"E");
	print_vector(stdout,f,size,"F");
	ds=(__m128 *)d;es=(__m128 *)e;
	fs = add_vec_sse(ds,es,size);
	print_vector_sse(stdout,fs,size,"F");
	_mm_free(d);_mm_free(e);_mm_free(f);_mm_free(fs);


	printf("\n-- Calculating formula\n");
	printf("G = H*A*D+E, where H - number, D and E - vectors, A - matrix\n\n");
	a = create_matrix(size,min_number,max_number);
	d = create_vector(size,min_number,max_number);
	e = create_vector(size,min_number,max_number);
	h = rand_f(min_number,min_number);
	printf("Number H is %i\n\n",(int)h);
	print_matrix(stdout,a,size,"A");
	print_vector(stdout,d,size,"D");
	print_vector(stdout,e,size,"E");
	b = mul_num_mat(h,a,size);
	f = mul_mat_vec(b,d,size);
	g = add_vec(f,e,size);
	print_vector(stdout,g,size,"G");
	as=(__m128 **)a;ds=(__m128 *)d;es=(__m128 *)e;
	hs=_mm_set1_ps(h);
	bs = mul_num_mat_sse(hs,as,size);
	fs = mul_mat_vec_sse(bs,ds,size);
	gs = add_vec_sse(fs,es,size);
	print_vector_sse(stdout,gs,size,"G");
	free_m(a,size);_mm_free(d);_mm_free(e);
	free_m(b,size);_mm_free(f);_mm_free(g);
	free_m_sse(bs,size);_mm_free(fs);_mm_free(gs);

	printf("\n-- Collecting statistics\n");
	printf("- Multiplying 2 matrices\n");
	clock_t t1,t2;
	double dur[2];
	FILE * file = fopen("1.csv","w");
	for (size=100;size<=1000;size+=100) {
		a = create_matrix(size,min_number,max_number);
		b = create_matrix(size,min_number,max_number);
		t1=clock();
		c = mul_mat(a,b,size);
		t2=clock();
		dur[0] = 1.0*(t2-t1)/CLOCKS_PER_SEC;
		as=(__m128 **)a;bs=(__m128 **)b;
		t1=clock();
		cs=mul_mat_sse(as,bs,size);
		t2=clock();
		dur[1] = 1.0*(t2-t1)/CLOCKS_PER_SEC;
		free_m(a,size);free_m(b,size);free_m(c,size);
		free_m_sse(cs,size);
		printf("%i,%lf,%lf\n",size,dur[0],dur[1]);
		fprintf(file,"%i,%lf,%lf\n",size,dur[0],dur[1]);
	}
	fclose(file);

	file = fopen("2.csv","w");
	printf("\n- Calculating formula\n");
	for (size=100;size<=4000;size+=100) {
		a = create_matrix(size,min_number,max_number);
		d = create_vector(size,min_number,max_number);
		e = create_vector(size,min_number,max_number);
		h = rand_f(min_number,min_number);
		t1=clock();
		b = mul_num_mat(h,a,size);
		f = mul_mat_vec(b,d,size);
		g = add_vec(f,e,size);
		t2=clock();
		dur[0] = 1.0*(t2-t1)/CLOCKS_PER_SEC;
		as=(__m128 **)a;ds=(__m128 *)d;es=(__m128 *)e;
		hs=_mm_set1_ps(h);
		t1=clock();
		bs = mul_num_mat_sse(hs,as,size);
		fs = mul_mat_vec_sse(bs,ds,size);
		gs = add_vec_sse(fs,es,size);
		t2=clock();
		dur[1] = 1.0*(t2-t1)/CLOCKS_PER_SEC;
		free_m(a,size);_mm_free(d);_mm_free(e);
		free_m(b,size);_mm_free(f);_mm_free(g);
		free_m_sse(bs,size);_mm_free(fs);_mm_free(gs);
		printf("%i,%lf,%lf\n",size,dur[0],dur[1]);
		fprintf(file,"%i,%lf,%lf\n",size,dur[0],dur[1]);
	}
	fclose(file);
	return 0;
}

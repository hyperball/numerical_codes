/*
This code can be used to calculate the coefficients of the cubic spline interpolation.
*/
#include<stdio.h>
#include<stdlib.h>

struct linked_node{
	double q;
	struct linked_node *next;
}linked_node;

struct array{
	int length;
	double *v;
}array;


int append(struct linked_node **p, double v);
int display_linked_node(struct linked_node *p);
int input(char *fn, struct linked_node **px, struct linked_node **py);

int get_size(struct linked_node *p);
int create_array(int n, struct array *p);
int linked2array(struct linked_node *pl, struct array *pa);
int cal_coeff(char *fn);

int append(struct linked_node **p, double v)
{
	struct linked_node *new_node = (struct linked_node*)malloc(sizeof(struct linked_node));
	struct linked_node *last = *p;
	new_node->q = v;
	new_node->next = NULL;
	if(NULL == (*p)){
		*p = new_node;
		return 0;
	}
	while(NULL != last->next)
		last = last->next;
	last->next = new_node;
	return 0;
}

int display_linked_node(struct linked_node *p)
{
	while(NULL != p){
		printf("%lf\t", p->q);
		p= p->next;
	} 
	printf("\n");
	return 0;
}

int get_size(struct linked_node *p)
{
	int cnt = 0;
	if(NULL == p) return cnt;
	while(NULL != p){
		p = p->next;
		cnt++;
	}
	return cnt;
}

int create_array(int n, struct array *p)
{
	p->length = n;
	p->v = (double *)malloc(n * sizeof(double));
	return 0;
}

int linked2array(struct linked_node *pl, struct array *pa)
{
	int cnt = 0;
	int i;
	cnt = get_size(pl);
	create_array(cnt, pa);
	if(NULL == pl)exit(EXIT_FAILURE);
	for(i = 0; i< cnt; i++){
		pa->v[i] = pl->q;
		pl = pl->next;
	}
	return 0;
}

int input(char *fn, struct linked_node **px, struct linked_node **py)
{
	FILE *fp;
	double a, b;
	a = b = 0.0;
	if(NULL == (fp = fopen(fn, "r"))){
		printf("DATAFILE WAS NOT FOUND! \n");
		exit(EXIT_FAILURE);
	}
	while(EOF != fscanf(fp, "%lf %lf", &a, &b)){
		append(px, a);
		append(py, b);
	}
	printf("\n");
	fclose(fp);
	return 0;
}

int cal_coeff(char *fn)
{
	struct linked_node *px, *py;
	px = py = NULL;
	struct array x, a;
	struct  array h, alf, l, mu, z;
	struct array b, c, d;

	input(fn, &px, &py);
	linked2array(px, &x);
	linked2array(py, &a);
	int len = x.length - 1 ;
	create_array(len, &h);
	create_array(len, &alf);
	create_array(len+1, &l);
	create_array(len+1, &mu);
	create_array(len+1, &z);
	create_array(len, &b);
	create_array(len+1, &c);
	create_array(len, &d);
	
	int i, j;
	for(i = 0; i < len; i++){
		h.v[i] = x.v[i+1] - x.v[i]; 
	}

	for(i = 1; i < len; ++i){
		alf.v[i] = 3.0 * (a.v[i+1] - a.v[i]) / h.v[i] - 3.0 * (a.v[i] - a.v[i-1]) / h.v[i-1];
	}

	l.v[0] = 1.0; 
	mu.v[0] = 0.0;
	z.v[0] = 0.0;

	for(i = 1; i < len ; ++i){
		l.v[i] = 2 * (x.v[i+1] - x.v[i-1]) - h.v[i-1] * mu.v[i-1];
		mu.v[i] = h.v[i] / l.v[i];
		z.v[i] = (alf.v[i] - h.v[i-1] * z.v[i-1]) /l.v[i];
	}
	l.v[len] = 1.0;
	z.v[len] = 0.0;
	c.v[len] = 0.0;
	for(j = len -1; j>=0; --j){
		c.v[j] = z.v[j] - mu.v[j] * c.v[j+1];
		b.v[j] = (a.v[j+1] - a.v[j]) / h.v[j] - h.v[j] * (c.v[j+1] + 2.0 * c.v[j]) / 3.0;
		d.v[j] = (c.v[j+1] -c.v[j]) / (3 * h.v[j]);
	}
	
	FILE *fo = fopen("coefficient.dat","w");
	if(NULL == fo){
		exit(EXIT_FAILURE);
	}
	
	for(i = 0; i < len; ++i){
		fprintf(fo, "%lf\t%lf\t%lf\t%lf\n", a.v[i], b.v[i], c.v[i], d.v[i]);
	}
	fclose(fo);
	return 0;
}

int main(int argc, char **argv)
{
	cal_coeff("in.txt");
	return 0;
}

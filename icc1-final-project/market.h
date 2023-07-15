#include <stdio.h>
#include <stdlib.h>

typedef struct {
	// SOLVE: Should a pounter be used?
	char name[64];
	int amount;
	double price;
} product;

typedef struct {
	product* stock;
	int stock_space;
	double balance;
} market;

int space_available(market* shop);

void end_task(void);
void parse_stock(market* shop);
void purge_stock(market* shop);
void create_empty_stock(market* shop,int initial_stock);
void check_stock(market* shop);
void select_operation(market*, int op);
void insert_stock_item(market* shop,char* name,int amount,double price);
void change_stock_size(market* shop,int size);
void increase_product_amount(int* stock_amount,int new_amount);
void modify_item_price(double* stock_price,double new_price);
void clear_item(product* item);
void check_balance(double balance);
void sell_item(market* shop);
void list_stock(market* shop);

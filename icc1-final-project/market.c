#include "market.h"
#include "string.h"

void end_task(void)
{
	printf("--------------------------------------------------\n");
}

void parse_stock(market* shop)
{
	FILE *data = fopen("data.bin", "rb");
	fread(&shop->stock_space,sizeof(int),1,data);
	fread(&shop->balance,sizeof(double),1,data);
        create_empty_stock(shop,shop->stock_space);
	for (int i = 0; i < shop->stock_space; i++)
	{
		fread(&shop->stock[i],sizeof(product),1,data);
 	}
        fclose(data);
}

void purge_stock(market* shop)
{
	FILE *data = fopen("data.bin", "wb");
    	if (data == NULL)
	{
        	printf("Failed to open the file.\n");
        	exit(EXIT_FAILURE);
    	}

	fwrite(&shop->stock_space,sizeof(int),1,data);
	fwrite(&shop->balance,sizeof(double),1,data);
	for (int i = 0; i < shop->stock_space; i++)
	{
		fwrite(&shop->stock[i],sizeof(product),1,data);
 	}
	free(shop->stock);
	exit(0);
}

void create_empty_stock(market* shop,int initial_stock)
{
	if (initial_stock == 0)
	{
		// what?
	}
	else
	{
		shop->stock = malloc(initial_stock * sizeof(product));
		for (int i = 0; i < initial_stock; i++)
		{
			clear_item(&shop->stock[i]);
 		}
	}
}

void clear_item(product* item)
{
	strcpy(item->name, "Vazio");
	item->amount = 0;
	item->price = 0.0f;
}

void check_stock(market* shop)
{
	FILE *data = fopen("data.bin", "rb");
	if (data == NULL)
	{
		// If openning the file fails it assumes the file do not exist
		int stock_size;
		double stock_balance;
		scanf("%d %lf",&stock_size,&stock_balance);
		shop->stock_space = stock_size;
		shop->balance = stock_balance;
        	create_empty_stock(shop,stock_size);
	}
	else
	{
		fclose(data);
		parse_stock(shop);
	}
}

void select_operation(market* shop,int op)
{
	int tag;
	char name[64];
	int amount;
	double price;
	switch (op)
	{
		case 153:
			scanf(" %s %d %lf",name,&amount,&price);
			//printf(" %s %d %lf\n",name,amount,price);
			insert_stock_item(shop,name,amount,price);
			break;
		case 134:
			scanf(" %d %d",&tag,&amount);
			increase_product_amount(&shop->stock[tag].amount,amount);
			break;
		case 157:
			scanf(" %d %lf",&tag,&price);
			//printf(" %d %lf\n",tag,price);
			modify_item_price(&shop->stock[tag].price,price);
			break;
		case 155:
			sell_item(shop);
			end_task();
			break;
		case 136:
			list_stock(shop);
			end_task();
			break;
		case 150:
			check_balance(shop->balance);
			end_task();
			break;
		case 139:
			purge_stock(shop);
			break;
 	}
}

void insert_stock_item(market* shop,char* name,int amount,double price)
{
	int count = 0;
	int avail = 0;
	for (int i = 0; i < shop->stock_space; i++)
	{
		if (shop->stock[i].price == 0.0f && shop->stock[i].amount == 0)
		{
			count++;
		}
	}
	for (int i = 0; i < shop->stock_space; i++)
	{
		if (shop->stock[i].price == 0.0f && shop->stock[i].amount == 0)
		{
			avail = i;
			break;
		}
	}
	if (count == 0)
	{
		avail = shop->stock_space;
		change_stock_size(shop,avail+1);
		strcpy(shop->stock[avail].name,name);
		shop->stock[avail].amount = amount;
		shop->stock[avail].price = price;
	}
	else
	{
		strcpy(shop->stock[avail].name,name);
		shop->stock[avail].amount = amount;
		shop->stock[avail].price = price;
	}
}

void change_stock_size(market* shop,int size)
{
	if(size == 0)
	{
		shop->stock = malloc(sizeof(product));
	}
	else
	{
		shop->stock = realloc(shop->stock, size);
	}
	if (shop->stock == NULL)
	{
	        printf("Unable to increase stock\n");
		free(shop->stock);
	        exit(EXIT_FAILURE);
	}
	shop->stock_space = size;
}
void increase_product_amount(int* stock, int amount)
{
	*stock += amount;
}

void modify_item_price(double* stock, double price)
{
	*stock = price;
}

void sell_item(market* shop)
{
	int val;
	double balance = 0;
	while (1)
	{
        	scanf(" %d", &val);
		if (val <= shop->stock_space)
		{
        		if (val == -1)
			{
            			break;
        		}
			if (shop->stock[val].amount != 0)
			{
				shop->stock[val].amount--;
				balance += shop->stock[val].price;
				printf("%s %.2lf\n",shop->stock[val].name,shop->stock[val].price);
			}
		}
	}
	shop->balance += balance;
	printf("Total: %.2lf\n",balance);
}

void list_stock(market* shop)
{
	for (int i = 0;  i < shop->stock_space; i++)
	{
		//printf("%d %s %d %lf\n",i,shop->stock[i].name,shop->stock[i].amount,shop->stock[i].price);
		printf("%d %s %d\n",i,shop->stock[i].name,shop->stock[i].amount);
 	}
}

void check_balance(double balance)
{
	printf("Saldo: %.2lf\n",balance);
}

int main(void)
{
	market shop;
	check_stock(&shop);
	while(1)
	{
		//SOLVE: add better checking
		char op[3] = {0};
		scanf("%s",op);
		if (op[0] != op[1])
		{
			select_operation(&shop,op[0] + op[1]);
		}
			
	}
	return 0;
}

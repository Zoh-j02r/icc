#include "market.h"
#include "string.h"

void end_task()
{
	printf("\n--------------------------------------------------\n");
}

void parse_stock(market* shop)
{
	// SOLVE: "Seu Zé" thinks that doing business with you was the greatest mistake 
	// of his entire life (He is 82 years old by the way)
	FILE *data = fopen("data.bin", "rb");
	fread(&shop->max_stock_space,sizeof(int),1,data);
        create_empty_stock(shop,shop->max_stock_space);
	for (int i = 0; i < shop->max_stock_space; i++)
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
		// SOLVE: The "Seu Zé" is losting money every time this application
		// crashes, do you really want to end up with his business?
        	printf("Failed to open the file.\n");
        	exit(EXIT_FAILURE);
    	}

	fwrite(&shop->max_stock_space,sizeof(int),1,data);
	for (int i = 0; i < shop->max_stock_space; i++)
	{
		// SOLVE: "Seu Zé" after a long day of work lost the entire stock data 
		fwrite(&shop->stock[i],sizeof(product),1,data);
 	}
	free(shop->stock);
	exit(0);
}

void create_empty_stock(market* shop,int initial_stock)
{
	shop->stock = malloc(initial_stock * sizeof(product));
	for (int i = 0; i < initial_stock; i++)
	{
		clear_item(&shop->stock[i]);
 	}
	shop->balance = 100.0f;
}

void clear_item(product* item)
{
	strcpy(item->name, "Vazio");
	item->amount = 0;
	item->price = 0.0f;
}

void check_stock(market* shop)
{
	// Must check if a stock file already exist, if not create a empty new stock
	FILE *data = fopen("data.bin", "rb");
	if (data == NULL)
	{
		// If openning the file fails it assumes the file do not exist
		int stock_size;
		scanf("%d",&stock_size);
		shop->max_stock_space = stock_size;
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
			// SOLVE: is there a way to make this better looking?
			scanf(" %s",name);
			scanf(" %d",&amount);
			scanf(" %lf",&price);
			insert_stock_item(shop,name,amount,price);
			break;
		case 134:
			scanf(" %d",&amount);
			scanf(" %d",&tag);
			increase_product_amount(&shop->stock[tag],amount);
			break;
		case 157:
			// SOLVE: get rid of this aux variable
			scanf(" %lf",&price);
			scanf(" %d",&tag);
			modify_item_price(&shop->stock[tag],price);
			break;
		case 155:
			sell_item(shop);
			break;
		case 136:
			list_stock(shop);
			break;
		case 150:
			check_balance(shop->balance);
			break;
		case 139:
			purge_stock(shop);
			break;
 	}
	if (op != 153)
	{
		end_task();
	}
}

void insert_stock_item(market* shop,char* name,int amount,double price)
{
	for (int i = 0; i < shop->max_stock_space; i++)
	{
		if (shop->stock[i].amount == 0)
		{
			
			strcpy(shop->stock[i].name,name);
			shop->stock[i].amount = amount;
			shop->stock[i].price = price;
			break;
		}
 	}
}

void change_stock_size(market* shop,int size)
{
	shop->stock = realloc(shop->stock, size);
	if (shop->stock == NULL)
	{
	        printf("Unable to increase stock\n");
	        free(shop);
	        exit(EXIT_FAILURE);
	}
	shop->max_stock_space = size;
}
void increase_product_amount(product* stock, int amount)
{
	stock->amount = amount;
}

void modify_item_price(product* stock, double price)
{
	stock->price = price;
}

void sell_item(market* shop)
{
	int val;
	double balance;
	while (1)
	{
        	scanf(" %d", &val);
		if (val <= shop->max_stock_space)
		{
        		if (val == -1)
			{
            			break;
        		}
			if (shop->stock[val].amount)
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
	for (int i = 0;  i < shop->max_stock_space; i++)
	{
		printf("%d %s %d\n",i,shop->stock[i].name,shop->stock[i].amount);
 	}
}

void check_balance(double balance)
{
	printf("Saldo: %.2lf",balance);
}

int main()
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

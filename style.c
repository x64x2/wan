// #define macros are UPPER_CASE.
#define GAUSS(n) ((n * (n+1))/2)

// pointers are prepended with a 'p'
char *p_sample;

// enums are CamelBack
enum GameState
{
	MAINMENU,
	LOADING,
	GAME
};

// structures are CamelBack
struct Simple
{
	int			number1;
	int			number2;
};

struct Complex
{
	// Members are in lower_case_underscored. Pointers are prepended
	// with a 'p'
	char			first;
	long			second;
	unsigned int		third;
	unsigned char		fourth;
	unsigned char		*p_fifth;
	unsigned char		**pp_sixth;
	
	// structures and enumerations last
	struct Simple		example;
	
	enum E
	{
		COOL,
		VERY_COOL,
		AWESOME,
		LEGENDARY
	}e;
};

// every scope should have their '{' go on a new line. Functions are in
// lower_case_underscored.
int do_something(int some_number, struct Complex *p_example)
{
	// if using C89, declare all variables and structures before
	// doing anything else. Use empty scopes to deallocate variables
	// and structures from the stack.
	{
		int thingy;
	
		if(some_number == thingy)
		{
			p_example->second--;
		}
	}
	
	return p_example->second;

}


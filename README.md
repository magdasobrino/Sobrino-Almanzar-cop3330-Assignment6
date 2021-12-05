# Sobrino-Almanzar-cop3330-Assignment6
User Guide Calculator 

Calculation Structure:
        Entry
        Print
        Quit
        Result
    
    Statement:
        Declaration
        Expression
    
    Declaration:
        "let" Name "=" Expression
        "const" Name "=" Expression

    Print:
        ;

    Quit:
        q 

    Expression:
        Term
        Expression + Term
        Expression - Term
    Term:
        Primary
        Term * Primary
        Term / Primary
        Term % Primary
    Primary:
        Number
        Name
		Name = Expression
        ( Expression )
        - Primary
        + Primary
    Number:
        floating-point-literal
	Name:
		[a-zA-Z][a-zA-Z_0-9]


# Python Parser Syntax Diagrams

## Function Definition
```
[def]-->[identifier]-->(-->[parameter_list]-->)-->:-->[block]
                         └-----------------┘

parameter_list:
[identifier]--┬----------------┬-->
             └-->,-->[identifier]

block:
[INDENT]-->[statement]--┬----------------┬-->[DEDENT]
                       └-->[statement]
```

## If Statement
```
[if]-->[expression]-->:-->[block]--┬--------------------------------┬-->
                                  └-->[elif]-->[expression]-->:-->[block]
                                  └-->[else]-->:-->[block]
```

## While Loop
```
[while]-->[expression]-->:-->[block]
```

## For Loop
```
[for]-->[identifier]-->[in]-->[expression]-->:-->[block]
```

## Expression
```
[term]--┬--------------------------------┬-->
        └-->[comparison_op]-->[term]

term:
[factor]--┬--------------------------------┬-->
          └-->[additive_op]-->[factor]

factor:
[primary]--┬--------------------------------┬-->
           └-->[multiplicative_op]-->[primary]

primary:
┌-->[identifier]--┬--------------------------------┬-->
├-->[number]
├-->[string]
└-->(-->[expression]-->)
```

## Assignment
```
[identifier]-->=-->[expression]
```

## Function Call
```
[identifier]-->(--┬--------------------------------┬-->)-->
                  └-->[expression]--┬------------┬
                                   └-->,-->[expression]
```

Note: These syntax diagrams represent the core grammar rules implemented in the parser. The actual implementation includes additional rules and error handling not shown in these simplified diagrams. 
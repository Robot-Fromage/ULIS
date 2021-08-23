# Operator Overloading
These rules are straight copied from [this thread on StackOverflow](https://stackoverflow.com/questions/4421706/what-are-the-basic-rules-and-idioms-for-operator-overloading) in the event that the page is taken down.

##The Three Basic Rules of Operator Overloading in C++

When it comes to operator overloading in C++, there are three basic rules you should follow. As with all such rules, there are indeed exceptions. Sometimes people have deviated from them and the outcome was not bad code, but such positive deviations are few and far between. At the very least, 99 out of 100 such deviations I have seen were unjustified. However, it might just as well have been 999 out of 1000. So you’d better stick to the following rules.

> Whenever the meaning of an operator is not obviously clear and undisputed, it should not be overloaded. Instead, provide a function with a well-chosen name.
> Basically, the first and foremost rule for overloading operators, at its very heart, says: Don’t do it. That might seem strange, because there is a lot to be known about operator overloading and so a lot of articles, book chapters, and other texts deal with all this. But despite this seemingly obvious evidence, there are only a surprisingly few cases where operator overloading is appropriate. The reason is that actually it is hard to understand the semantics behind the application of an operator unless the use of the operator in the application domain is well known and undisputed. Contrary to popular belief, this is hardly ever the case.

> Always stick to the operator’s well-known semantics.
> C++ poses no limitations on the semantics of overloaded operators. Your compiler will happily accept code that implements the binary + operator to subtract from its right operand. However, the users of such an operator would never suspect the expression a + b to subtract a from b. Of course, this supposes that the semantics of the operator in the application domain is undisputed.

> Always provide all out of a set of related operations.
> Operators are related to each other and to other operations. If your type supports a + b, users will expect to be able to call a += b, too. If it supports prefix increment ++a, they will expect a++ to work as well. If they can check whether a < b, they will most certainly expect to also to be able to check whether a > b. If they can copy-construct your type, they expect assignment to work as well.

##The Decision between Member and Non-member

The binary operators = (assignment), [] (array subscription), -> (member access), as well as the n-ary () (function call) operator, must always be implemented as member functions, because the syntax of the language requires them to.

Other operators can be implemented either as members or as non-members. Some of them, however, usually have to be implemented as non-member functions, because their left operand cannot be modified by you. The most prominent of these are the input and output operators << and >>, whose left operands are stream classes from the standard library which you cannot change.

For all operators where you have to choose to either implement them as a member function or a non-member function, use the following rules of thumb to decide:

> If it is a unary operator, implement it as a member function.
> If a binary operator treats both operands equally (it leaves them unchanged), implement this operator as a non-member function.
> If a binary operator does not treat both of its operands equally (usually it will change its left operand), it might be useful to make it a member function of its left operand’s type, if it has to access the operand's private parts.

Of course, as with all rules of thumb, there are exceptions. If you have a type

    enum Month {Jan, Feb, ..., Nov, Dec}

and you want to overload the increment and decrement operators for it, you cannot do this as a member functions, since in C++, enum types cannot have member functions. So you have to overload it as a free function. And operator<() for a class template nested within a class template is much easier to write and read when done as a member function inline in the class definition. But these are indeed rare exceptions.

(However, if you make an exception, do not forget the issue of const-ness for the operand that, for member functions, becomes the implicit this argument. If the operator as a non-member function would take its left-most argument as a const reference, the same operator as a member function needs to have a const at the end to make *this a const reference.)
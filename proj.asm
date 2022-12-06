// Md Ahnaf Masud Eusa
// 30075798
// Project part 2
//--------------------

output1: .string "Invalid number of arguments passed\n"
output2: .string "M should be in the range of 5 to 20\n"
output3: .string "N should be in the range of 5 to 20\n"
output4: .string "%.2f\t"
output5: .string "\n"
output6: .string "%s\n"
output7: .string "$\t"
output8: .string "Percentage of negative numbers: %.2f%%\n"
output9: .string "X "
output10: .string "\nUncovered Board:\n"
output11: .string "\nCovered Board:\n"
output12: .string "Percentage of positive numbers: %.2f%%\n"
output13: .string "Percentage of special tiles: %.2f%%\n"
        .balign 4
        .global main

        define(temp_r, x19)
        define(rows_r, x20)
        define(cols_r, x21)
        define(i_r, x22)
        define(j_r, x23)
        define(alloc_r, x24)
        define(offset_r, x25)
        define(playerName_r, x26)
	define(posPercent_r, x27)

main:
        stp     x29, x30, [sp, -16]!
        mov     x29, sp

        //storing the command line arguments
        mov     temp_r, x1                      //store the command line arguments in a temp register

        //validation for number of arguments
        cmp     x0, 3                           //compare number of arguments to 3
        b.lt    wrongInput1                     //if less, branch to wrongInput1
        cmp     x0, 4                           //compare number of arguments to 4
        b.gt    wrongInput1                     //if more, branch to wrongInput1

        //storing 1st argument
        ldr     x0, [temp_r, 8]                 //load the 1st input passed onto x0
        mov     playerName_r, x0                //move the integer to rows_r

        //storing 2nd argument
        ldr     x0, [temp_r, 16]                //load the 2nd input passed onto x0
        bl      atoi                            //convert the argument to integer
        mov     rows_r, x0                      //move the integer to cols_r

        //validation for 2nd argument
        cmp     rows_r, 10                       //compare it with 5
        b.lt    wrongInput2                     //if less, branch to wrongInput3
        cmp     rows_r, 200                      //compare it with 20
        b.gt    wrongInput2                     //if more, branch to wrongInput3

        //storing 3rd argument
        ldr     x0, [temp_r, 24]                //load the 2nd input passed onto x0
        bl      atoi                            //convert the argument to integer
        mov     cols_r, x0                      //move the integer to cols_r

        //validation for 3rd argument
        cmp     cols_r, 10                       //compare it with 5
        b.lt    wrongInput3                     //if less, branch to wrongInput3
        cmp     cols_r, 200                      //compare it with 20
        b.gt    wrongInput3                     //if more, branch to wrongInput3

        //seeding the rand
        mov     x0, xzr                         //move 0 to x0
        bl      time                            //call the function time(0)
        bl      srand                           //call the function srand to have different numbers

	ldr	x0, =output6			//load print statement
	mov	x1, playerName_r		//pass playerName as argument
	bl	printf				//print player name

	//fmov	d0, #0.25
	//ldr	x0, =output4
	//bl	printf

	ldr	x0, =output10			//load print statement
	bl	printf				//print "Uncovered Board:"
	
	mov	temp_r, xzr			//initialize temp
	mov	posPercent_r, xzr		//initialize positive percentage

initialize:


loop1:
        mov     x0, xzr				//move 0 to x0
        mov     x1, 0                           //move 0 to x1(lower bound)
        mov     x2, 15                          //move 9 to x2(upper bound)

        bl      randomNum                       //call function randomNum(m,n)

	mov	x0, xzr				//move 0 to x0
	bl	rand				//call rand()
	and	x0, x0, 0xF			//AND 15

	cmp	x0, 6				//compare x0 and 6
	b.gt	next				//if greater than, branch to next

	//NEGATIVE FLOAT
	fneg	d0, d0				//negating d0
	add	temp_r, temp_r, 1		//incrementing temp
	b	next2				//branch to print statement

next:	
	cmp	x0, 9				//compare x0 to 9
	b.gt	next2				//if greater than, branch to next2

	//POWERUP	
	ldr	x0, =output7			//load print statement
	bl	printf				//print "  $  "
	b	next3				//branch to next3

next2:
        //POSITIVE FLOAT
        ldr     x0, =output4                    //load print message "%d "
        bl      printf                          //printing random number
	add	posPercent_r, posPercent_r, 1	//increment positive percentage

next3:
        add     j_r, j_r, 1                     //increment j
        b       cmp2                            //branch to cmp2

cmp1:
        cmp     j_r, cols_r                     //compare j to cols
        b.lt    loop1                           //if less, branch to loop1

        add     i_r, i_r, 1                     //increment i
        mov     j_r, xzr                        // j = 0
        ldr     x0, =output5                    //load print message "\n"
        bl      printf                          //print "\n"

cmp2:
        cmp     i_r, rows_r                     //compare i to rows
        b.lt    cmp1                            //if less, branch to cmp1

	sub	posPercent_r, posPercent_r, temp_r	//posPercent= posPercent - temp

//Negative Num Percentage
percentage:
	mov	x0, 100				//x0 = 100
	scvtf	d0, temp_r			//d0 = temp
	scvtf	d1, rows_r			//d1 = rows
	scvtf	d2, cols_r			//d2 = cols
	scvtf	d3, x0				//d3 = 100.0

	fmul	d4, d1, d2			//d4 = rows * cols
	fdiv	d0, d0, d4			//d0 = d0(temp)/d4(rows*cols)
	fmul	d0, d0, d3			//d0 = d0 * 100.00
	//d0 = (temp/(rows*cols))*100
	
	ldr	x0, =output8			//load print statement
	bl	printf				//print	percentage of negative number
//Positive Num Percentage
        mov     x0, 100                         //x0 = 100
        scvtf   d0, posPercent_r                //d0 = posPercent
        scvtf   d1, rows_r                      //d1 = rows
        scvtf   d2, cols_r                      //d2 = cols
        scvtf   d3, x0                          //d3 = 100.0

        fmul    d4, d1, d2                      //d4 = rows * cols
        fdiv    d0, d0, d4                      //d0 = d0(posPercent)/d4(rows*cols)
        fmul    d0, d0, d3                      //d0 = d0 * 100.00
        //d0 = (posPercent/(rows*cols))*100

        ldr     x0, =output12                    //load print statement
        bl      printf                          //print percentage of negative number
//Special Tiles Percentage
	mov	x0, 100				//x0 = 100
	add	x1, posPercent_r, temp_r	//x1 = posPercent + temp
	sub	x1, x0, x1			//x1 = 100 - x1
	scvtf   d0, x1		                //d0 = x1
        scvtf   d1, rows_r                      //d1 = rows
        scvtf   d2, cols_r                      //d2 = cols
        scvtf   d3, x0                          //d3 = 100.0

        fmul    d4, d1, d2                      //d4 = rows * cols
        fdiv    d0, d0, d4                      //d0 = d0(specialtilePercentage)/d4(rows*cols)
        fmul    d0, d0, d3                      //d0 = d0 * 100.00
        //d0 = (posPercent/(rows*cols))*100

        ldr     x0, =output13                    //load print statement
        bl      printf


//Covered Board
	ldr	x0, =output11			//load print statement
	bl	printf				//print "Covered Board:"
	
board:
	mov	i_r, 0				//move 0 to i
	mov	j_r, 0				//move 0 to j

loop2:
        ldr     x0, =output9                    //load print message "X "
        bl      printf                          //printing X

        add     j_r, j_r, 1                     //increment j
        b       cmp4                            //branch to cmp2

cmp3:
        cmp     j_r, cols_r                     //compare j to cols
        b.lt    loop2                           //if less, branch to loop1

        add     i_r, i_r, 1                     //increment i
        mov     j_r, xzr                        // j = 0
        ldr     x0, =output5                    //load print message "\n"
        bl      printf                          //print "\n"

cmp4:
        cmp     i_r, rows_r                     //compare i to rows
        b.lt    cmp3                            //if less, branch to cmp1


        ldp x29, x30, [sp], 16                  //ending program
        ret
//---------------------------------------------------------------------------------------


//RANDOM NUM
//--This function takes in two integers as input and returns a random number in between--
randomNum:
        stp     x29, x30, [sp, -16]!
        mov     x29, sp

        //x1 is lower bound
	//d1 is lower bound in float
        //x2 is upper bound
	//d2 is upper bound in float
	//x3 is neg

	//x7 is randMax
	//d7 is randMax in float
	//x6 is the random number 
	//d6 is the random number in float

while:
        mov     x0, xzr                         //move 0 to x0
        bl      rand                            //call function rand()
        and     x0, x0, 0xF			//AND with 15 to make it in range of 15
	sub	x0, x0, 1			//number -1

	mov	x6, x0				//moving random number to x6
	scvtf	d6, x6				//store x6 as float in d6
	
	mov	x7, 2147483647			//store RAND_MAX in x7
	scvtf	d7, x7				//store x7 as float in d7

        scvtf   d8, x1                          //store x1(lower bound) as float in d8
        scvtf   d9, x2                          //store x2(upper bound) as float in d9

	mov	x0, xzr				//move 0 to xo
	bl	rand				//call function rand()
	scvtf	d0, x0				//storing the random number in d0 as float

	fdiv	d0, d0, d7			//rand/RAND_MAX
	fadd	d0, d0, d6			//rand(float between 0.0 and 1.0) + d6(float between 0.0 and 15.0)

	fcmp	d0, 0.0				//validation
	b.eq	while

        fcmp    d0, 0.0				//validation
        b.lt    while

        ldp     x29, x30, [sp], 16              //end funtion
        ret                                     //return

//-------------------------------------------------------------------------------------------------

wrongInput1:
        ldr x0, =output1                        //printing an exit message
        bl printf
        ldp x29, x30, [sp], 16                  //ending program
        ret

wrongInput2:
        ldr x0, =output2                        //printing an exit message
        bl printf
        ldp x29, x30, [sp], 16                  //ending program
        ret

wrongInput3:
        ldr x0, =output3                        //printing an exit message
        bl printf
        ldp x29, x30, [sp], 16                  //ending program
        ret

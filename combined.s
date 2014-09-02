.equ PS2_KEYBOARD_DATA, 0x10000100
.equ PS2_KEYBOARD_CONTROL, 0x10000104
.equ SEG_LOWER, 0x10000020
.equ TIMER, 0x10002000 #set timer address
.equ PERIOD, 25000000 #set timer period
.equ MOVEPERIOD, 2500000 #set timer period
.equ SHOOTPERIOD, 10000000 #set timer period
.equ ADDR_JP1, 0x10000060
.equ DEFAULT_VAL, 0xb


.section .text
.global main

 main:
 #store ra on stack
		
		## r21 will be score  ##
		## r22 will be player ##
		## r23 will be round  ##
		
		movi r22, 0x0 # 0x0 will be player 1 and 0x0 is player 2
		movi r23, 0x0 # initialize to round 1 will divide yo get round number
		
		subi sp, sp,4
		stw ra, 0(sp)

		call clear_screen

		# restore ra, pop stack
		ldw ra, 0(sp)
		addi sp,sp,4

		#store ra on stack
		subi sp, sp,4
		stw ra, 0(sp)

		call initialization

		# restore ra, pop stack
		ldw ra, 0(sp)
		addi sp,sp,4
		
			movia r8, TIMER
			movia r9, %lo(PERIOD)
			stwio r9, 8(r8)
			movui r9, %hi(PERIOD)
			stwio r9, 12(r8)
			stwio r0, 0(r8)
			movi r9, 0b111
			stwio r9, 4(r8)
			
			
			movia r16, PS2_KEYBOARD_DATA #  4(r8) is PS2_KEYBOARD_CONTROL
			movia r18, SEG_LOWER
			
			ldwio r10, 4(r16)
			movi r11, 0x1
			or r10, r11, r10
			stwio r10, 4(r16)
			#comment
			movi r11, 0x81 //0x80 for key board and 0x1 for timer
			wrctl ctl3, r11
			movi r11, 0x1
			wrctl ctl0, r11 #enable external interrupts

		
		sub r10, r10, r10
		sub r11, r11, r11
		
		movia r8, ADDR_JP1
	
		addi sp, sp, -0x4 #push
		stw r22, 0(sp)
		movia  r22, 0x07f557ff      /* set direction for motors and sensors to output and sensor data register to inputs*/
		stwio  r22, 4(r8)	
		ldw r22, 0(sp) #pop
		addi sp, sp, 0x4
		
		movia r12, 0xffffffff #disable sensor 3
		stwio r12, 0(r8)
	
	continue:
	movi r4, 20
	beq r23, r4, ENDGAME
	
	movia r8, ADDR_JP1
	movi r9, DEFAULT_VAL
  
	READ_SENSORS:
	addi sp, sp, -0x4
	stw r11, 0(sp)
	movi r11, 0x0
	wrctl ctl0, r11 #disable external interrupts
	ldw r11, 0(sp)
	addi sp, sp, 0x4
	
		#read leftmost_sensor
		ori r12, r12, 0xffff #disable sensor 3
		sth  r12, 0(r8)
		sth  r12, 2(r8)
		andi r12, r12, 0xfbff #enable sensor 0
		sth  r12, 0(r8)
		
		READ_POLL_1:
			ldwio  r5, 0(r8)
			srli   r5, r5, 11	#bit 11 equals valid bit for sensor 0      
			andi   r5, r5, 0x1
			bne    r0, r5, READ_POLL_1
	
		ldwio  r10, 0(r8) 
		srli   r10, r10, 27 #shift for sensor value in lower 4 bits 
		addi r10, r10, -0x1
		andi r10,r10,0xffef

		#read left_sensor
		ori r12, r12, 0xffff #disable sensor 3
		sth  r12, 2(r8)
		sth  r12, 0(r8)
		andi r12, r12, 0xefff #enable sensor 1
		sth  r12, 0(r8)
		
		READ_POLL_2:
			ldwio  r5, 0(r8)
			srli   r5, r5, 13	#bit 13 equals valid bit for sensor 1      
			andi   r5, r5, 0x1
			bne    r0, r5, READ_POLL_2
	
		ldwio  r11, 0(r8) 
		srli   r11, r11, 27 #shift for sensor value in lower 4 bits 
		andi r11,r11,0xffef
		
		#read right_sensor
		ori r12, r12, 0xffff #disable sensor 3
		sth  r12, 2(r8)
		sth  r12, 0(r8)
		andi r12, r12, 0xbfff #enable sensor 2
		sth r12, 0(r8)
		
		READ_POLL_3:
			ldwio  r5, 0(r8)
			srli   r5, r5, 15 	#bit 15 equals valid bit for sensor 2*/           
			andi   r5, r5, 0x1
			bne    r0, r5, READ_POLL_3
			
		ldwio  r13, 0(r8) 
		srli   r13, r13, 27 #shift for sensor value in lower 4 bits 
		andi r13,r13,0xffef
		
		#read rightmost_sensor
		ori r12, r12, 0xffff #disable sensor 3
		sth  r12, 2(r8)
		sth  r12, 0(r8)
		sth r12, 0(r8)
		andi r12, r12, 0xfffe #enable sensor 3
		sth r12, 2(r8)
		
		READ_POLL_4:
			ldwio  r5, 0(r8)
			srli   r5, r5, 17 	#bit 17 equals valid bit for sensor 3*/           
			andi   r5, r5, 0x1
			bne    r0, r5, READ_POLL_4
			
		ldwio  r14, 0(r8) 
		srli   r14, r14, 27 #shift for sensor value in lower 4 bits 
		#addi r14, r14, -0x1
		andi r14,r14,0xffef
		
		addi sp, sp, -0x4
		stw r11, 0(sp)
		movi r11, 0x1
		wrctl ctl0, r11 #disable external interrupts
		ldw r11, 0(sp)
		addi sp, sp, 0x4
		
		bne r10, r9, CHECK_ALIGNMENT
		bne r11, r9, CHECK_ALIGNMENT
		bne r13, r9, CHECK_ALIGNMENT
		beq r14, r9, READ_SENSORS #ball has yet to launch 
		
	CHECK_ALIGNMENT:
			
			bne r14, r10, skip
			
			beq r11, r10, case_strike
			
			skip:
			
		#else if right_sensor < left_sensor
			blt r13, r11, case_right
			
		#else if right_sensor > left_sensor
			bgt r13, r11, case_left
			
			blt r14, r10, case_right
			
			bgt r14, r10, case_left
	
		br READ_SENSORS

	case_right:
		movi r15, 0x4
		
		#if right_sensor > value
		bgt r13, r11, pin_arrangement1
		
		bgt r14, r13, pin_arrangement2
		
		blt r14, r15, pin_arrangement3
			
		#send appropriate signal to VGA
	
		br READ_SENSORS
		
	case_left: 
		movi r15, 0x4
		
		#if left_sensor > value
		bgt r11, r13, pin_arrangement4
		
		bgt r10, r11, pin_arrangement5

		blt r10, r15, pin_arrangement6
		
		br continue
	
	case_strike:
		movi r4, 10
		mov r5, r23
		mov r6, r22
		
		call fillinscore
		mov r4, r22
		call blink
		
		movi r4, 0x0
		beq r22, r4 , invert
		movi r22, 0x0
		br over
		invert:
		movi r22, 0x1
		over:
		call timer
		addi r23, r23, 1
		br continue
		
	pin_arrangement1:
		movi r4, 1
		mov r5, r23
		mov r6, r22
		
		call fillinscore
		mov r4, r22
		call blink
		movi r4, 0x0
		
		beq r22, r4 , invert1
		movi r22, 0x0
		br over1
		invert1:
		movi r22, 0x1
		over1:
		
		call timer
		
		addi r23, r23, 1 # next round
		br continue
		
	pin_arrangement2:
		movi r4, 2
		mov r5, r23
		mov r6, r22
		
		call fillinscore
		mov r4, r22
		call blink
		movi r4, 0x0
		
		beq r22, r4 , invert2
		movi r22, 0x0
		br over2
		invert2:
		movi r22, 0x1
		over2:
		call timer
		addi r23, r23, 1
		br continue
		
	pin_arrangement3:
		movi r4, 3
		mov r5, r23
		mov r6, r22
		
		call fillinscore
		mov r4, r22
		call blink
		movi r4, 0x0
		beq r22, r4 , invert3
		movi r22, 0x0
		br over3
		invert3:
		movi r22, 0x1
		over3:
		call timer
		addi r23, r23, 1
		br continue
		
	pin_arrangement4:
		movi r4, 5
		mov r5, r23
		mov r6, r22
		
		call fillinscore
		mov r4, r22
		call blink
		
		movi r4, 0x0
		beq r22, r4 , invert4
		movi r22, 0x0
		br over4
		invert4:
		movi r22, 0x1
		over4:
		
		call timer
		addi r23, r23, 1
		br continue
		
	pin_arrangement5:
		movi r4, 7
		mov r5, r23
		mov r6, r22
		
		call fillinscore
		mov r4, r22
		call blink
		movi r4, 0x0
		beq r22, r4 , invert5
		movi r22, 0x0
		br over5
		invert5:
		movi r22, 0x1
		over5:
		call timer
		addi r23, r23, 1
		br continue
		
	pin_arrangement6:
		movi r4, 9
		mov r5, r23
		mov r6, r22
		
		call fillinscore
		mov r4, r22
		call blink
		movi r4, 0x0
		beq r22, r4 , invert6
		movi r22, 0x0
		br over6
		invert6:
		movi r22, 0x1
		over6:
		call timer
		addi r23, r23, 1
		br continue
	
	/** END **/	

	ENDGAME:
		movi r4, 2
		call blink
		infloop:
			br infloop
	

	/** SUBROUTINES **/
	
	timer:
		addi sp,sp,-4*2
		stw r16, 0(sp)
		stw r17, 4(sp)
	 
		movia r16, TIMER
		movui r17, %lo(PERIOD)
		stwio r17,8(r16)
		movui r17, %hi(PERIOD)
		stwio r17, 12(r16)
		
		stwio r0, 0(r16)
		movi r17, 0x6
		stwio r17,4(r16)
		
		POLL:
			ldwio r17, 0(r16)
			andi r17,r17,0x1
			beq r17,r0,POLL
			stwio r0,0(r16)
			
		ldw r16, 0(sp)
		ldw r17, 4(sp)
		addi sp,sp,4*2
	ret
	
	shoot_timer:
		addi sp,sp,-4*2
		stw r16, 0(sp)
		stw r17, 4(sp)
	 
		movia r16, TIMER
		movui r17, %lo(SHOOTPERIOD)
		stwio r17,8(r16)
		movui r17, %hi(SHOOTPERIOD)
		stwio r17, 12(r16)
		
		stwio r0, 0(r16)
		movi r17, 0x6
		stwio r17,4(r16)
		
		POLL2:
			ldwio r17, 0(r16)
			andi r17,r17,0x1
			beq r17,r0,POLL2
			stwio r0,0(r16)
			
		ldw r16, 0(sp)
		ldw r17, 4(sp)
		addi sp,sp,4*2
		
	ret
	
	move_timer:
		addi sp,sp,-4*2
		stw r16, 0(sp)
		stw r17, 4(sp)
	 
		movia r16, TIMER
		movui r17, %lo(MOVEPERIOD)
		stwio r17,8(r16)
		movui r17, %hi(MOVEPERIOD)
		stwio r17, 12(r16)
		
		stwio r0, 0(r16)
		movi r17, 0x6
		stwio r17,4(r16)
		
		POLL3:
			ldwio r17, 0(r16)
			andi r17,r17,0x1
			beq r17,r0,POLL3
			stwio r0,0(r16)
			
		ldw r16, 0(sp)
		ldw r17, 4(sp)
		addi sp,sp,4*2
		
	ret
	
	/** INTERRUPTS **/

	.section .exceptions, "ax"

ihandler:
	addi sp, sp, -0x4
	stw r12, 0(sp)
	movia r16, PS2_KEYBOARD_DATA #  4(r8) is PS2_KEYBOARD_CONTROL
	movia r18, SEG_LOWER
	movia r8, ADDR_JP1
	
	rdctl r6, ctl4
	andi et, r6, 0x80
	mov r17, r0
	beq et, r0, timer_handler
	

keyboard:
	ldwio r20, 0(r16)
	andi r20, r20, 0xff

UP_KEY:
	cmpeqi r21, r20, 0x1d
	beq r21, r0, DOWN_KEY
	movi r19, 0x71
	stwio r19, 0(r18)
	move_right:
		ori  r12, r12, 0xffff
		andi  r12, r12, 0xfffb
		sth r12, 0(r8)
		addi sp,sp,-4 
		stw r12, 0(sp)
		call move_timer
		ldw r12, 0(sp)
		addi sp,sp,4
		ori  r12, r12, 0xffff
		sth r12, 0(r8)
	
	br exit_handler

DOWN_KEY:
	cmpeqi r21, r20, 0x1b
	beq r21, r0, SPACE_BAR
	movi r19, 0x7100
	stwio r19, 0(r18)
	move_left:
		ori  r12, r12, 0xffff
		andi  r12, r12, 0xfff3
		sth r12, 0(r8)
		addi sp,sp,-4
		stw r12, 0(sp)
		call move_timer
		ldw r12, 0(sp)
		addi sp,sp,4
		ori  r12, r12, 0xffff
		sth r12, 0(r8)
		
	br exit_handler

SPACE_BAR:
	cmpeqi r21, r20, 0x29
	beq r21, r0, exit_handler
	movi r19, 0x77
	stwio r19, 0(r18)
	shoot:
		ori  r12, r12, 0xffff
		andi  r12, r12, 0xfffe
		sth r12, 0(r8)
		addi sp,sp,-4
		stw r12, 0(sp)
		call shoot_timer
		ldw r12, 0(sp)
		addi sp,sp,4
		ori  r12, r12, 0xffff
		sth r12, 0(r8)
	
	br exit_handler
	
	timer_handler:
		andi et, r6, 0x1 #check for interrupt in irq0 - timer
		beq et, r0, exit_handler
		
		#update periodic score and flash player indicator
		
		movi r4, 10
		mov r5, r23
		mov r6, r22
		call fillinscore
		mov r4, r22
		call blink
		
		movia et, TIMER
		stwio r0, 0(et)
	
exit_handler:
	ldw r12, 0(sp)
	addi sp, sp, 0x4
	subi ea, ea, 4
	eret
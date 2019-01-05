	.text

label_1:

	beq	$4,$5,label_1
	bne	$4,$5,label_2
	ble	$4,$5,label_1
	bleu	$4,$5,label_2
	blt	$4,$5,label_1
	bltu	$4,$5,label_2
	bge	$4,$5,label_1
	bgeu	$4,$5,label_2
	bgt	$4,$5,label_1
	bgtu	$4,$5,label_2

label_2:

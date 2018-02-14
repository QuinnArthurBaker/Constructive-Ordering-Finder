for j in {1..100};
do
	echo $j;
	./fact_sub_multi 12 2>&1 | grep "3856";
done


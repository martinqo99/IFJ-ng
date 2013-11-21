<?php
	// Program 1: Vypocet faktorialu (iterativne)
	// Hlavni telo programu
	$x = put_string("Zadejte cislo pro vypocet faktorialu\n");
	$a = get_string();
	$a = intval($a);

	if ($a < 0)
	{
		$x = put_string("Faktorial nelze spocitat\n");
	}
	else
	{
		$vysl = 1;
		while ($a > 0)
		{
			$vysl = $vysl * $a;
			$a = $a - 1;
		}
		$x = put_string("Vysledek je: ", $vysl, "\n");
	}
?>

<?php
	// Program 2: Vypocet faktorialu (rekurzivne)
	// Hlavni telo programu
	$z = put_string("Zadejte cislo pro vypocet faktorialu\n");
	$a = get_string();
	$a = intval($a);

	// Definice funkce pro vypocet hodnoty faktorialu
	function factorial($n)
	{
		if ($n < 2)
		{
			$result = 1;
		}
		else
		{
			$decremented_n = $n - 1;
			$temp_result = factorial($decremented_n);
			$result = $n * $temp_result;
		}
		return $result;
	}

	if ($a < 0) // Pokracovani hlavniho tela programu
	{
		$message = "Faktorial nelze spocitat\n";
	}
	else
	{
		$vysl = factorial($a);
		$message = "Vysledek je: " . $vysl . "\n";
	}
	$zzz = put_string($message);
?>




<?php
	/* Program 3: Prace s retezci a vestavenymi funkcemi */
	function MyMain($str)
	{
		$str1 = $str;
		$str2 = $str1 . ", ktery jeste trochu obohatime";
		
		$x = put_string($str1, "\n", $str2, "\n");
		$p = find_string($str2, "text");
		$x = put_string("Pozice retezce \"text\" v \$str2: ", $p, "\n");
		$x = put_string("Zadejte posloupnost vsech malych pismen a-h, ");
		$x = put_string("aby se pismena v posloupnosti neopakovala:\n");
		
		$str1 = get_string();
		$str2 = sort_string($str1);
		
		while ($str2 !== "abcdefgh") 
		{
			$x = put_string("Spatne zadana posloupnost, zkuste znovu:\n");
			$str1 = get_string();
			$str2 = sort_string($str1);
		}
		return 0;
	}
	$foo = MyMain("Toto je nejaky text v programu jazyka IFJ13");
?>

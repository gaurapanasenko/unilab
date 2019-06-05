<?php
include "config.php";
$title = "Лабораторна робота №3";
$heading = "Лабораторна робота №3";
$mysqli = new mysqli($mysql_host, $mysql_username, $mysql_password, $mysql_database);
if ($mysqli->connect_errno) {
	include "header.php"; ?>
		<div class="alert alert-secondary" role="alert">
			Не вдається з'єднатися з базою даних.
		</div>
	<?php include "footer.php";
	exit();
}
$path = parse_url($_SERVER['REQUEST_URI'], PHP_URL_PATH);
$path_check = substr($path, 0, strlen($base_path));
if ($path_check == $base_path) {
	$path = explode("/", substr($path, strlen($base_path) + 1));
	if (in_array($path[0], array("edit", "list", "delete")))
		include "$path[0].php";
	else include "list.php";
} else {
	include "header.php"; ?>
		<div class="alert alert-secondary" role="alert">
			Ця сторінка не існує. <a href='<?=$base_path?>'>Повернутися додому.</a>
		</div>
	<?php include "footer.php";
}
$mysqli->close();
?>

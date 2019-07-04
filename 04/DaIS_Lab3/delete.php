<?php
if (isset($_REQUEST["id"]) and is_numeric($_REQUEST["id"]) and intval($_REQUEST["id"]) != 0) {
	$id = intval($_REQUEST["id"]);
	$query = "DELETE FROM `tree` WHERE `id` = $id";
	$mysqli->query($query);
}
header("Location: $base_path");
?>

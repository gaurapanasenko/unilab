<?php
$title = "Додати новий елемент";
$heading = "Додати новий елемент";
$btn = "Додати";
$id = 0;
$submit = isset($_REQUEST["submit"]);
if (isset($_REQUEST["parent_id"]) and (is_numeric($_REQUEST["parent_id"]) || empty($_REQUEST["parent_id"]))) {
	$parent_id = intval($_REQUEST["parent_id"]);
	if ($submit and $parent_id) {
		$query = "SELECT `id`, `name` FROM `tree` WHERE `id` = $parent_id";
		if ($result = $mysqli->query($query) and $result->fetch_assoc()) {
			$result->free();
		} else {
			$error = "Неправильний батьківський елемент";
		}
	}
} elseif ($submit) {
	$error = "Неправильний батьківський елемент";
}
if (!empty($_REQUEST["name"])) {
	$name = $_REQUEST["name"];
} elseif ($submit) {
	$error = "Немає назви";
}
if (isset($_REQUEST["id"]) and is_numeric($_REQUEST["id"]) and intval($_REQUEST["id"]) != 0) {
	$title = "Змінити елемент";
	$heading = "Змінити елемент";
	$btn = "Змінити";
	$id = intval($_REQUEST["id"]);
	$query = "SELECT * FROM `tree` WHERE `id` = $id";
	if ($result = $mysqli->query($query) and $data = $result->fetch_assoc()) {
		if (!isset($name)) $name = $data["name"];
		if (!isset($parent_id)) $parent_id = $data["parent_id"];
		$result->free();
	} else {
		include "header.php"; ?>
			<div class="alert alert-secondary" role="alert">
				Не існує такого елемента. <a href="<?=$base_path?>edit/0">Додати новий.</a>
			</div>
		<?php include "footer.php";
		exit();
	}
} elseif ($submit and isset($_REQUEST["id"]) and !is_numeric($_REQUEST["id"])) {
	$error = "Неправильний індекс елементу";
}
if (!isset($name)) $name = "";
if (!isset($parent_id)) $parent_id = 0;
if ($submit and !$parent_id) {
	$parent_id = "NULL";
}
if ($submit and !isset($error)) {
	if ($id) {
		$query = "UPDATE `tree` SET `parent_id`=$parent_id, `name`='$name' WHERE `id`=$id";
	} else {
		$query = "INSERT INTO `tree` (`parent_id`, `name`) VALUES ($parent_id, '$name')";
	}
	if ($mysqli->query($query) === TRUE) {
		header("Location: $base_path");
	} else {
		$error = "Виникла помилка при додаванні або зміні елемента: " . $mysqli->error;
	}
}
//~ $query = "SELECT `id`, `name` FROM `tree` WHERE `id` != $id";
//~ if ($result = $mysqli->query($query)) {
	//~ $parents = $result->fetch_all();
	//~ $result->free();
//~ } else {
	//~ include "header.php";
		//~ <div class="alert alert-secondary" role="alert">
			//~ Виникла помилка при завантаженні списку елементів.
		//~ </div>
	//~ include "footer.php";
	//~ exit();
//~ }
include "header.php";
	if (!empty($error)) { ?>
		<div class="alert alert-secondary" role="alert"><?=$error?></div>
	<?php } ?>
	<form action="<?=$base_path?>edit" method="get">
		<input type="hidden" name="id" value="<?=$id?>">
		<input type="hidden" name="parent_id" value="<?=$parent_id?>">
		<div class="form-group row">
			<label for="name" class="col-sm-2 col-form-label">Назва</label>
			<div class="col-sm-10">
				<input type="text" class="form-control" id="name" name="name" value="<?=$name?>">
			</div>
		</div>
		<div class="form-group row">
			<div class="col-sm-10">
				<button type="submit" class="btn btn-primary" name="submit"><?=$btn?></button>
			</div>
		</div>
</form>
<?php include "footer.php"; ?>

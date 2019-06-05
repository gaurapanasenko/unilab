<?php

function print_error() {
	include "header.php"; ?>
		<div class="alert alert-secondary" role="alert">
			Виникла помилка при завантаженні списку елементів.
		</div>
	<?php include "footer.php";
	exit();
}

function format_data($id = NULL, $raw_data) {
	$filtered = array_filter($raw_data, function($value) use ($id) {
		return $value[1] == $id;
	});
	foreach($filtered as &$value) {
		$value[3] = format_data($value[0], $raw_data);
	}
	return $filtered;
}

function get_root_path($id) {
	global $mysqli;
	$data = NULL;
	while ($id) {
		$query = "SELECT `id`, `parent_id`, `name` FROM `tree` WHERE `id`=$id";
		if ($result = $mysqli->query($query)) {
			$dt = $result->fetch_all();
			$dt[0][3] = $data;
			$data = $dt;
			$id = $data[0][1];
		} else {
			print_error();
		}
	}
	return $data;
}

function get_childs($id = NULL) {
	global $mysqli;
	$query = "SELECT `id`, `parent_id`, `name` FROM `tree` WHERE `parent_id`=$id";
	if (!($result = $mysqli->query($query))) {
		print_error();
	}
	$data = $result->fetch_all();
	foreach($data as &$value) {
		$value[3] = get_childs($value[0]);
	}
	return $data;
}

function print_data($data) {
	global $base_path;
	$id1 = !empty($_REQUEST["id1"]) ? $_REQUEST["id1"] : "";
	$id2 = !empty($_REQUEST["id2"]) ? $_REQUEST["id2"] : "";
	?>
	<ul>
	<?php foreach($data as $value) { ?>
		<li class="list-element">
			<?=$value[2]?>
			<small class="ml-4">
			<a class="list-link" href="<?=$base_path?>edit?id=<?=$value[0]?>"
			data-toggle="tooltip" title="Змінити елемент"><i class="fa fa-edit"></i></a>
			<a class="list-link" href="<?=$base_path?>edit?parent_id=<?=$value[0]?>"
			data-toggle="tooltip" title="Додати дочірній елемент"><i class="fa fa-plus-circle"></i></a>
			<a class="list-link" href="<?=$base_path?>delete?id=<?=$value[0]?>"
			data-toggle="tooltip" title="Видалити елемент"><i class="fa fa-trash"></i></a>
			<a class="list-link" href="<?=$base_path?>list?mode=root&id=<?=$value[0]?>"
			data-toggle="tooltip" title="Знайти шлях до кореневого елементу"><i class="fa fa-square-root-alt"></i></a>
			<a class="list-link" href="<?=$base_path?>list?mode=childs&id=<?=$value[0]?>"
			data-toggle="tooltip" title="Вивести усі дорічні елементи"><i class="fa fa-leaf"></i></a>
			<a class="list-link" href="<?=$base_path?>list?mode=path&id1=<?=$value[0]?>&id2=<?=$id2?>"
			data-toggle="tooltip" title="Знайти шлях з цього елементу"><i class="fa fa-fast-backward"></i></a>
			<a class="list-link" href="<?=$base_path?>list?mode=path&id1=<?=$id1?>&id2=<?=$value[0]?>"
			data-toggle="tooltip" title="Знайти шлях до цього елементу"><i class="fa fa-fast-forward"></i></a>
			</small>
		<?php print_data($value[3]); ?>
		</li>
	<?php } ?>
	</ul>
<?php }

$heading = $heading . " <a class='list-link list-link-head' href='{$base_path}edit' data-toggle='tooltip' title='Додати новий елемент'><i class='fa fa-plus-circle'></i></a>";
if ($_REQUEST["mode"] == "root" and !empty($_REQUEST["id"]) and is_numeric($_REQUEST["id"]) and intval($_REQUEST["id"]) != 0) {
	$id = intval($_REQUEST["id"]);
	$data = get_root_path($id);
} elseif ($_REQUEST["mode"] == "childs" and !empty($_REQUEST["id"]) and is_numeric($_REQUEST["id"]) and intval($_REQUEST["id"]) != 0) {
	$id = $_REQUEST["id"];
	$query = "SELECT `id`, `parent_id`, `name` FROM `tree` WHERE `id`=$id";
	if (!($result = $mysqli->query($query))) {
		print_error();
	}
	$data = $result->fetch_all();
	$data[0][3] = get_childs($id);
} elseif ($_REQUEST["mode"] == "path" and !empty($_REQUEST["id1"]) and is_numeric($_REQUEST["id1"]) and intval($_REQUEST["id1"]) != 0 and !empty($_REQUEST["id2"]) and is_numeric($_REQUEST["id2"]) and intval($_REQUEST["id2"]) != 0) {
	$id1 = intval($_REQUEST["id1"]);
	$id2 = intval($_REQUEST["id2"]);
	$arr1 = get_root_path($id1);
	$arr2 = get_root_path($id2);
	while (!empty($arr1[0][3][0][0]) and !empty($arr2[0][3][0][0]) and $arr1[0][3][0][0] == $arr2[0][3][0][0]) {
		$arr1 = $arr1[0][3];
		$arr2 = $arr2[0][3];
	}
	if ($arr1[0][0] == $arr2[0][0]) {
		$data = $arr1;
		$data[0][3] = array_merge(empty($arr1[0][3]) ? array() : $arr1[0][3], empty($arr2[0][3]) ? array() : $arr2[0][3]);
	} else {
		$data = array_merge($arr1, $arr2);
	}
} else {
	$query = "SELECT `id`, `parent_id`, `name` FROM `tree`";
	if ($result = $mysqli->query($query)) {
		$raw_data = $result->fetch_all();
		$result->free();
	} else {
		print_error();
	}
	$data = format_data(NULL, $raw_data);
}
include "header.php";
print_data($data);
include "footer.php";
?>

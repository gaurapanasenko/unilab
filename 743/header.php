<!doctype html>
<html lang="en">
	<head>
		<!-- Required meta tags -->
		<meta charset="utf-8">
		<meta name="viewport" content="width=device-width, initial-scale=1,
			shrink-to-fit=no">

		<!-- Bootstrap CSS -->
		<link rel="stylesheet"
			href="/css/bootstrap.min.css"
			integrity="sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T"
			crossorigin="anonymous">

		<!-- Font Awesome CSS -->
		<link href="/css/all.min.css" rel="stylesheet">

		<!-- Optional JavaScript -->
		<!-- jQuery first, then Popper.js, then Bootstrap JS -->
		<script src="/js/jquery-3.3.1.slim.min.js"
		integrity="sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo"
		crossorigin="anonymous"></script>
		<script src="/js/popper.min.js"
		integrity="sha384-UO2eT0CpHqdSJQ6hJty5KVphtPhzWj9WO1clHTMGa3JDZwrnQq4sF86dIHNDz0W1"
		crossorigin="anonymous" defer></script>
		<script src="/js/bootstrap.min.js"
		integrity="sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM"
		crossorigin="anonymous" defer></script>
		<script type="text/javascript" defer>
			$(document).ready(function(){$('[data-toggle="tooltip"]').tooltip()});
		</script>

		<title><?=$title?></title>

		<style>
			.list-element {
				font-size: 1.5rem;
			}
			.list-link {
				color: green !important;
				font-size: 0.5rem;
			}
			.list-link-head {
				font-size: inherit;
			}
		</style>
	</head>
	<body>
		<nav class="navbar navbar-expand-lg navbar-light bg-light shadow-sm">
			<div class="container">
				<a class="navbar-brand" href="<?=$base_path?>">Лабораторна робота №3</a>
				<button class="navbar-toggler" type="button"
					data-toggle="collapse" data-target="#navbarSupportedContent"
					aria-controls="navbarSupportedContent" aria-expanded="false"
					aria-label="Toggle navigation">
					<span class="navbar-toggler-icon"></span>
				</button>
				<div class="collapse navbar-collapse"
					id="navbarSupportedContent">
					<ul class="navbar-nav mr-auto">
						<li class="nav-item">
							<a class="nav-link" href="<?=$base_path?>">Головна</a>
						</li>
					</ul>
				</div>
			</div>
		</nav>
		<div class="container">
			<h1><?=$heading?></h1>

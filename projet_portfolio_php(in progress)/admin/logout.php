<?php
require_once("../intialize.php");
require_once '../globalConfig.php';
session_start();
$_SESSION["auth"] = [];
session_destroy();
header("Location:".LOGIN_URL);

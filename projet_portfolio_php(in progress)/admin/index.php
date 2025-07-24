<?php
require_once("../intialize.php");
require_once('../auth/auth.php');
require_once("../globalConfig.php");

?>

<!DOCTYPE html>
<html lang="fr">
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css">
    <link rel="stylesheet" href="../assets/css/style.css">
    <script src="../assets/js/js.js"></script>
    <title></title>
</head>
<body>
<?php include './partials/navbar.php' ?>
<div class="content">
    <?php
    if (!isset($_GET['page'])) {
        ?>
        welcome to my portfolio
        <?php
    }
    ?>

    <?php

    if (isset($_GET['page'])) {
        $pathToFile = $_GET['page'] . '/' . $_GET['page'] . '.php';
        if (file_exists($pathToFile)) {
            include_once $pathToFile;
        }
    } else {
        echo 'here 2';
    }
    ?>
</div>
</body>
</html>


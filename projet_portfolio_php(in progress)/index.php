<html>
<?php
$languages = ['fr', 'en'];

if (isset($_GET['lang'])) {
    $_SESSION['lang'] = $_GET['lang'];
} else {
    $_SESSION['lang'] = 'fr';
}
require_once 'lang/lang_'.$_SESSION['lang'].'.php';

?>


<header lang="fr">
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1">
<!--        <link rel="stylesheet"-->
<!--              href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css">-->
        <link rel="stylesheet" href="assets/css/style.css">
        <link rel="stylesheet" href="https://fonts.googleapis.com/icon?family=Material+Icons">
        <link rel="stylesheet" href="https://fonts.googleapis.com/css?family=Sofia">
        <link href="https://fonts.googleapis.com/css2?family=Roboto+Flex:opsz,wght@8..144,100&display=swap" rel="stylesheet">

        <script src="assets/js/js.js"></script>
        <title></title>
    </head>
</header>

<body>
<nav>
    <div class="navbar">
        <div class="logo"><a href="#">My Portfolio</a></div>
        <ul class="menu">
            <li><a href="#Home"><?php echo $lang['Accueil'] ?></a></li>
            <li><a href="#About"><?php echo $lang['À propos de moi'] ?></a></li>
            <li><a href="#projects"><?php echo $lang['Projets'] ?></a></li>
            <li>
                <a href="?lang=fr">Fr</a>
                -
                <a href="?lang=en">En</a></li>
        </ul>
    </div>
</nav>
<?php

require_once("intialize.php");
require_once("globalConfig.php");
$result = [];
$qry = $conn->query("SELECT * FROM about");
if ($qry) {
    $result['about'] = $qry->fetch();
}
$qryp = $conn->query("SELECT * FROM project");
if ($qryp) {
    $result['projects'] = $qryp->fetchAll();
}


?>
<section id="Home">
    Hello I'm <br><div style="color: orangered; text-transform:uppercase;"><?php echo $result['about']['fullname'] ?? ""; ?></div>
    & this is My <div style="color: orangered">&lt;Portfolio/&gt; </div>
    <br>

</section>
<section id="About">
    <h2><?php echo $lang['À propos de moi'] ?></h2>
    <h3>
        <?php echo $result['about']['fullname']." - ".$result['about']['age']." years - ".$result['about']['title']."</br>"; ?>
    </h3>
    <p>
        <?php
        echo stripslashes($result['about']['abouttext_'.$_SESSION['lang']]);
        ?>
    </p>
    <h2>Contact</h2>
    <p>
    <span class="material-icons">call</span> <?php echo $result['about']['contact']; ?> <br/>
    <span class="material-icons">mail</span> <?php echo $result['about']['email']; ?>
    </p>
</section>
<section id="projects">
    <h2>
        <?php echo $lang['Mes projets'] ?>
    </h2>
    <?php

    foreach ($result['projects'] as $project) {
        echo "<div class='project'>";
        echo "<h3>".$project['projectname']." - ".$project['client']."</h3>";
        echo "<p>".$project['description_'.$_SESSION['lang']]."</p>";
        echo "</div>";
    }

    ?>

</section>
<div class="button">
    <a href="#Home">Top</a>
</div>

</body>

<footer>

</footer>
</html>
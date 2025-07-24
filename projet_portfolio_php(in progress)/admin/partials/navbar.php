<div class="topnav" id="myTopnav">
    <a href="#" class="active">My Portfolio</a>
    <a href="?page=about">À propos</a>
    <a href="?page=projects">Projets</a>
    <a href="<?php echo ADMIN_URL.'/logout.php' ?>" style="float: right">Se déconnecter</a>
    <a href="javascript:void(0);" class="icon" onclick="navBarChange()">
        <i class="fa fa-bars"></i>
    </a>
</div>

<script>
    var ADMIN_URL = '<?php echo ADMIN_URL ?>';
</script>
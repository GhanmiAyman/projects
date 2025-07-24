<?php
require_once("../intialize.php");
require_once '../globalConfig.php';
$error_messages = "";
if (isset($_POST['username']) && isset($_POST['password'])) {
    session_start();
    extract($_POST);
    $qry = $conn->exec("SELECT * from user where username = '$username' and password = md5('$password') ");
    if ($qry->num_rows > 0) {
        $result = $qry->fetch_array();
        $_SESSION['auth'] = $result;
        header('Location:' . ADMIN_URL);
    } else {
        $error_messages = "Invalid Credentials";
    }
}
?>


<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="../assets/css/login.css">
</head>
<body>

<div class="login-page">
    <div class="form">
        <div class="login">
            <div class="login-header">
                <h3>LOGIN</h3>
                <p>Please enter your credentials to login.</p>
            </div>
        </div>
        <?php
        if (!empty($error_messages)) {
            ?> <div class="validation-message"><?php  echo $error_messages; ?></div> <?php
        }
        ?>
        <form action="#" method="POST" class="login-form">
            <input type="text" name="username" placeholder="username" value="<?php if(isset($_POST['username'])) { echo $_POST['username'];} ?>"/>
            <input type="password" name="password" placeholder="password"/>
            <button type="submit">login</button>
        </form>
    </div>
</div>
</body>
</html>

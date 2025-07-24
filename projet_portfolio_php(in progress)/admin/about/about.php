<?php
include '../classes/About.php';

$about = new About();
$about->getAbout();

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $about->save();
}

?>
<h1 class="centre-title">À propos de moi</h1>

<div class="container">
    <form action="#" method="POST" id="form_about">
        <div class="row">
            <div class="col-25">
                <label for="fullname">Nom et prénom</label>
            </div>
            <div class="col-75">
                <input type="text" id="fullname" name="fullname" required value="<?php echo $about->getFullname() ?>">
            </div>
        </div>
        <div class="row">
            <div class="col-25">
                <label for="age">Âge</label>
            </div>
            <div class="col-75">
                <input type="number" id="age" name="age" required value="<?php echo $about->getAge() ?>">
            </div>
        </div>
        <div class="row">
            <div class="col-25">
                <label for="about_title">Titre du poste</label>
            </div>
            <div class="col-75">
                <input type="text" id="about_title" name="title" required value="<?php echo $about->getTitle() ?>">
            </div>
        </div>
        <div class="row">
            <div class="col-25">
                <label for="contact">Contact</label>
            </div>
            <div class="col-75">
                <input type="text" id="contact" name="contact" required value="<?php echo $about->getContact() ?>">
            </div>
        </div>
        <div class="row">
            <div class="col-25">
                <label for="email">Email</label>
            </div>
            <div class="col-75">
                <input type="email" id="email" name="email" required value="<?php echo $about->getEmail() ?>">
            </div>
        </div>
        <div class="row">
            <div class="col-25">
                <label for="aboutme_fr">À propos de moi</label>
            </div>
            <div class="col-75">
                <textarea id="aboutme_fr" name="about_text_fr" rows="10" cols="50" required ><?php echo $about->getAboutTextFr() ?></textarea>
            </div>
        </div>
        <div class="row">
            <div class="col-25">
                <label for="aboutme_en">À propos de moi (en)</label>
            </div>
            <div class="col-75">
                <textarea id="aboutme_en" name="about_text_en" rows="10" cols="50" required ><?php echo $about->getAboutTextEn() ?></textarea>
            </div>
        </div>
        <div class="row">
            <input type="submit" value="Sauvegarder">
        </div>
    </form>
</div>

<script>

    const form = document.getElementById('form_about');

    form.addEventListener('submit', event => {
        event.preventDefault();
        formValide();
        event.currentTarget.submit();
    });

    function formValide() {
        inputVerification("fullname");
        inputVerification("age");
        inputVerification("about_title");
        inputVerification("contact");

        return true;
    }
</script>

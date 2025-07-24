<?php

include '../classes/Project.php';

$project = new Project();
$result = [];

if (isset($_POST['action']) && $_POST['action'] == "delete") {
    $project->delete();
    exit();
}

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $project->save();
    echo "<meta http-equiv='refresh' content='0'>";
} else {
    $qry = $conn->query("SELECT * from project ");
    if ($qry) {
        $result = $qry->fetchAll();
    }
}

?>

<h1 class="centre-title">Projets</h1>

<!--<h2 class="centre-title">Ajouter un projet</h2>-->
<button onclick="showFrom()" class="button1">Ajouter un projet</button>

<div id="form_add_project" style="display: none">
    <div class="container">
        <form action="#" id="form_project" method="POST">
            <div class="row">
                <div class="col-25">
                    <label for="projectname">Nom du projet</label>
                </div>
                <div class="col-75">
                    <input type="text" id="projectname" name="projectname" placeholder="project name" required>
                </div>
            </div>
            <div class="row">
                <div class="col-25">
                    <label for="client">Client</label>
                </div>
                <div class="col-75">
                    <input type="text" id="client" name="client" placeholder="client" required>
                </div>
            </div>
            <div class="row">
                <div class="col-25">
                    <label for="description_fr">Description</label>
                </div>
                <div class="col-75">
                    <textarea id="description_fr" name="description_fr" placeholder="description" style="height:200px" required></textarea>
                </div>
            </div>
            <div class="row">
                <div class="col-25">
                    <label for="description_en">Description (en)</label>
                </div>
                <div class="col-75">
                    <textarea id="description_en" name="description_en" placeholder="description" style="height:200px" required></textarea>
                </div>
            </div>
            <div class="row">
                <input type="submit" value="Sauvegarder">
            </div>
        </form>
    </div>
</div>

<h2 class="centre-title">List des projets</h2>

<div class="container">
    <?php

    if (!empty($result)) {
        ?>
        <table id="table-projects">
            <tr>
                <th>Nom du projet</th>
                <th>Client</th>
                <th>Description</th>
                <th></th>
            </tr>
            <?php
            foreach ($result as $element)
            {
                echo "<tr id='table-tr-$element[0]'>";
                echo "<td>$element[1]</td>";
                echo "<td>$element[2]</td>";
                echo "<td>$element[3]</td>";
                echo "<td><button type='button' onclick='delete_project($element[0])'>x</button></td>";
                echo "</tr>";
            }
            ?>
        </table>
        <?php

    }

    ?>
</div>

<script>
    function showFrom()
    {
        var div = document.getElementById("form_add_project");
        if (div.style.display === "none") {
            div.style.display = "block";
        } else {
            div.style.display = "none";
        }
    }

    function delete_project(id){
        if (confirm("Are you sure to delete this element!")) {
            const xhttp = new XMLHttpRequest();
            xhttp.open("POST", ADMIN_URL+'/?page=projects');
            xhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
            xhttp.send("id="+id+"&action=delete");

            var i = document.getElementById("table-tr-"+id) .rowIndex;
            document.getElementById("table-projects").deleteRow(i);
        }
    }

    const form = document.getElementById('form_project');

    form.addEventListener('submit', event => {
        event.preventDefault();
        formValide();
        event.currentTarget.submit();
    });

    function formValide() {
        inputVerification("projectname");
        inputVerification("client");

        return true;
    }

</script>

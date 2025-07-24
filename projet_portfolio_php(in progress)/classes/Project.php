<?php

class Project extends DBConnection
{
    public $id;
    public $projectname;
    public $client;
    public $description_fr;
    public $description_en;

    public function save()
    {
        extract($_POST);
        $qry = $this->conn->prepare("INSERT INTO `project`(`projectname`, `client`, `description_fr`, `description_en`)
                                VALUES(:projectname, :client, :description_fr, :description_en)");
        $qry->bindParam(":projectname", $projectname);
        $qry->bindParam(":client", $client);
        $qry->bindParam(":description_fr", $description_fr);
        $qry->bindParam(":description_en", $description_en);
        $qry->execute();
        if ($qry) {
            return true;
        }
        return false;
    }

    public function delete()
    {
        extract($_POST);
        $qry = $this->conn->exec("DELETE FROM project where id = $id");
        if ($qry) {
            return true;
        }
        return false;
    }

    /**
     * @return mixed
     */
    public function getId()
    {
        return $this->id;
    }

    /**
     * @param mixed $id
     */
    public function setId($id): void
    {
        $this->id = $id;
    }

    /**
     * @return mixed
     */
    public function getProjectname()
    {
        return $this->projectname;
    }

    /**
     * @param mixed $projectname
     */
    public function setProjectname($projectname): void
    {
        $this->projectname = $projectname;
    }

    /**
     * @return mixed
     */
    public function getClient()
    {
        return $this->client;
    }

    /**
     * @param mixed $client
     */
    public function setClient($client): void
    {
        $this->client = $client;
    }

    /**
     * @return mixed
     */
    public function getDescriptionFr()
    {
        return $this->description_fr;
    }

    /**
     * @param mixed $description_fr
     */
    public function setDescriptionFr($description_fr): void
    {
        $this->description_fr = $description_fr;
    }

    /**
     * @return mixed
     */
    public function getDescriptionEn()
    {
        return $this->description_en;
    }

    /**
     * @param mixed $description_en
     */
    public function setDescriptionEn($description_en): void
    {
        $this->description_en = $description_en;
    }


}
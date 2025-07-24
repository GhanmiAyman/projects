<?php

class About extends DBConnection
{
    public $title;
    public $about_text_fr;
    public $about_text_en;
    public $fullname;
    public $age;
    public $contact;
    public $email;

    public function save() {
        extract($_POST);
        $qry = $this->conn->query("SELECT count(*) FROM about");
        if ($qry->fetchColumn() <= 0) {
            $qry = $this->conn->prepare("INSERT INTO `about`(`title`, `abouttext_fr`,`abouttext_en`, `fullname`, `age`, `contact`, `email`) VALUES(:title, :about_text_fr,:about_text_en, :fullname, :age, :contact, :email) ");
            $qry->bindParam(":title", $title);
            $qry->bindParam(":about_text_fr", $about_text_fr);
            $qry->bindParam(":about_text_en", $about_text_en);
            $qry->bindParam(":fullname", $fullname);
            $qry->bindParam(":age", $age);
            $qry->bindParam(":contact", $contact);
            $qry->bindParam(":email", $email);

            if ($qry->execute()) {
                $this->title = $title;
                $this->about_text_fr = $about_text_fr;
                $this->about_text_en = $about_text_en;
                $this->fullname = $fullname;
            }
        } else {
            $qry = $this->conn->prepare("UPDATE about set title=:title, abouttext_fr = :about_text_fr,abouttext_en = :about_text_en, fullname=:fullname, age=:age, contact=:contact, email=:email");
            $qry->bindParam(":title", $title);
            $qry->bindParam(":about_text_fr", $about_text_fr);
            $qry->bindParam(":about_text_en", $about_text_en);
            $qry->bindParam(":fullname", $fullname);
            $qry->bindParam(":age", $age);
            $qry->bindParam(":contact", $contact);
            $qry->bindParam(":email", $email);
            if ($qry->execute()) {
                $this->title = $title;
                $this->about_text_fr = $about_text_fr;
                $this->about_text_en = $about_text_en;
                $this->fullname = $fullname;
                $this->age = $age;
                $this->contact = $contact;
                $this->email = $email;
            }
        }
    }

    public function getAbout() {
        $qry = $this->conn->query("SELECT * FROM about");
        $result = $qry->fetch();
        if ($result) {
            $this->title = $result['title'];
            $this->about_text_fr = stripslashes($result['abouttext_fr'] ?? "");
            $this->about_text_en = stripslashes($result['abouttext_en'] ?? "");
            $this->fullname = $result['fullname'];
            $this->age = $result['age'];
            $this->contact = $result['contact'];
            $this->email = $result['email'];
        } else {
            $this->title = "";
            $this->about_text_fr = "";
            $this->about_text_en = "";
            $this->fullname = "";
            $this->age = "";
            $this->contact = "";
            $this->email = "";
        }

    }

    /**
     * @return mixed
     */
    public function getTitle()
    {
        return $this->title;
    }

    /**
     * @param mixed $title
     */
    public function setTitle($title): void
    {
        $this->title = $title;
    }

    /**
     * @return mixed
     */
    public function getAboutTextFr()
    {
        return $this->about_text_fr;
    }

    /**
     * @param mixed $about_text_fr
     */
    public function setAboutTextFr($about_text_fr): void
    {
        $this->about_text_fr = $about_text_fr;
    }

    /**
     * @return mixed
     */
    public function getAboutTextEn()
    {
        return $this->about_text_en;
    }

    /**
     * @param mixed $about_text_en
     */
    public function setAboutTextEn($about_text_en): void
    {
        $this->about_text_en = $about_text_en;
    }

    /**
     * @return mixed
     */
    public function getFullname()
    {
        return $this->fullname;
    }

    /**
     * @param mixed $fullname
     */
    public function setFullname($fullname): void
    {
        $this->fullname = $fullname;
    }

    /**
     * @return mixed
     */
    public function getAge()
    {
        return $this->age;
    }

    /**
     * @param mixed $age
     */
    public function setAge($age): void
    {
        $this->age = $age;
    }

    /**
     * @return mixed
     */
    public function getContact()
    {
        return $this->contact;
    }

    /**
     * @param mixed $contact
     */
    public function setContact($contact): void
    {
        $this->contact = $contact;
    }

    /**
     * @return mixed
     */
    public function getEmail()
    {
        return $this->email;
    }

    /**
     * @param mixed $email
     */
    public function setEmail($email): void
    {
        $this->email = $email;
    }
}
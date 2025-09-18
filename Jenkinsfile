pipeline {
    agent any
    stages {
       stage('build') {
          steps {
             echo 'First Stage'
          }
       }
       stage('test') {
           steps {
              sh 'ceedling'
           }
       }
    }
 }

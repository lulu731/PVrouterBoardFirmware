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
    post {
        failure {
            mail bcc:'', body: 'Jenkins reports a failed pipeline', cc: '', from: 'jenkins', replyTo: '', subject: 'Pipeline failed', to: 'lulu@msikatana'
        }
    }
 }

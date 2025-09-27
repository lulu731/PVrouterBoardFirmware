pipeline {
   agent any
   stages {
      stage('test') {
         steps {
            sh 'ceedling'
         }
      }
      stage('build') {
         steps {
            sh '/home/lulu/.platformio/penv/bin/pio run -e jenkins'
         }
      }
   }
   post {
      failure {
            mail bcc:'', body: "Jenkins reports a failed pipeline : #${BUILD_NUMBER} in ${BRANCH_NAME}", cc: '', from: 'jenkins', replyTo: '', subject: 'Pipeline failed', to: 'lulu@msikatana'
      }
   }
}

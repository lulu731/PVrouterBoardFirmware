pipeline {
   agent { label 'linux'}
   environment {
      GITHUB_CREDS = credentials('838f205e-7093-420e-9f0a-b308b8bd3202')
      REMOTE_URL   = "https://x-access-token:$GITHUB_CREDS_PSW@github.com/$GITHUB_CREDS_USR/PVrouterBoardFirmware.git"
   }
   stages {
      stage('run container'){
         when { environment name: 'NODE_NAME', value :'server'}
         steps {
            sh 'echo node name is $NODE_NAME'
            sh './jenkins/run_container.sh'
         }
      }
      stage('test') {
         steps {
            sh './jenkins/ceedling_test.sh $NODE_NAME'
         }
      }
      stage('build') {
         steps {
            sh './jenkins/platformio_build.sh $NODE_NAME'
         }
      }
   }
   post {
      always {
         script {
            if (env.NODE_NAME == 'server') {
               sh './jenkins/stop_container.sh'
            }
         }
      }
      success {
         script {
            if (env.BRANCH_NAME == 'develop') {
               sh 'git push $REMOTE_URL "HEAD:refs/heads/develop"'
            }
         }
      }
      failure {
         mail bcc:'', body: "Jenkins reports a failed pipeline : #${BUILD_NUMBER} in ${BRANCH_NAME}", cc: '', from: 'jenkins', replyTo: '', subject: 'Pipeline failed', to: 'lulu@msikatana'
      }
   }
}

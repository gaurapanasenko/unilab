import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Timeline 1.0
import QtGraphicalEffects 1.0

Page {
    id: page
    width: 600
    height: 400
    anchors.fill: parent

    title: qsTr("PixyWorld")

    Item {
        id: scene
        width: 512
        height: 338
        transform: Scale {
            xScale: page.width / scene.width
            yScale: page.height / scene.height
        }

        Image {
            id: skyImage
            source: "sky.png"
            anchors.fill: parent
            smooth: false
            visible: false
        }

        BrightnessContrast {
            id: sky
            anchors.fill: skyImage
            source: skyImage
            brightness: -0.7
            contrast: 0
        }

        Image {
            id: sun
            source: "sun.png"
            smooth: false
            x: 224
            y: 0
        }

        Image {
            id: groundImage
            source: "ground.png"
            anchors.fill: parent
            smooth: false
            visible: false
        }

        BrightnessContrast {
            id: ground
            anchors.fill: groundImage
            source: groundImage
            brightness: -0.7
            contrast: 0
        }
    }

    Timeline {
        id: timeline
        animations: [
            TimelineAnimation {
                id: timelineAnimation
                duration: 5000
                loops: -1
                running: true
                to: 1000
                from: 0
            }
        ]
        startFrame: 0
        enabled: true
        endFrame: 1000

        KeyframeGroup {
            target: sun
            property: "x"
            Keyframe {
                value: 518
                frame: 251
            }

            Keyframe {
                value: 224
                frame: 499
            }

            Keyframe {
                value: -70
                frame: 750
            }

            Keyframe {
                value: 518
                frame: 0
            }
        }

        KeyframeGroup {
            target: sun
            property: "y"
            Keyframe {
                value: 70
                frame: 250
            }

            Keyframe {
                easing.bezierCurve: [0.11, 0.989, 0.222, 0.994, 1, 1]
                value: 0
                frame: 500
            }

            Keyframe {
                easing.bezierCurve: [0.779, 0.000643, 0.889, 0.00527, 1, 1]
                value: 60
                frame: 750
            }

            Keyframe {
                value: 80
                frame: 0
            }
        }

        KeyframeGroup {
            target: ground
            property: "brightness"
            Keyframe {
                value: -0.7
                frame: 0
            }

            Keyframe {
                value: -0.7
                frame: 250
            }

            Keyframe {
                value: 0
                frame: 350
            }

            Keyframe {
                value: 0
                frame: 650
            }

            Keyframe {
                value: -0.7
                frame: 750
            }
        }

        KeyframeGroup {
            target: sky
            property: "brightness"
            Keyframe {
                value: -0.7
                frame: 0
            }

            Keyframe {
                value: -0.7
                frame: 250
            }

            Keyframe {
                value: 0
                frame: 350
            }

            Keyframe {
                value: 0
                frame: 650
            }

            Keyframe {
                value: -0.7
                frame: 750
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.75}D{i:5}D{i:7}D{i:8}
}
##^##*/


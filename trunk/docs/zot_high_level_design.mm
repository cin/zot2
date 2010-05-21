<map version="0.8.1">
<!-- To view this file, download free mind mapping software FreeMind from http://freemind.sourceforge.net -->
<node CREATED="1243830391783" ID="Freemind_Link_45102737" MODIFIED="1243831450796" TEXT="Zot&#xa;System Manager&#xa;SimTime Master&#xa;Main Entity List&#xa;State Driven - menu, paused, running, console, etc&#xa;">
<node CREATED="1243830471709" FOLDED="true" ID="_" MODIFIED="1243831122167" POSITION="right" TEXT="System&#xa;each system runs in its own thread&#xa;recv&apos;s only msgs that it registers for&#xa;can publish messages to the event distributor (typically the master interface in charge of passing the messages to all other systems)&#xa;collection of components (for data/serialization)&#xa;factory based creation (components must support this inherently)&#xa;">
<cloud/>
<node CREATED="1243831241665" ID="Freemind_Link_1905386885" MODIFIED="1243831248863" TEXT="Input"/>
<node CREATED="1243831255441" ID="Freemind_Link_320838144" MODIFIED="1243831262191" TEXT="Physics"/>
<node CREATED="1243831263267" ID="Freemind_Link_1376403495" MODIFIED="1243831272735" TEXT="Renderer"/>
<node CREATED="1243831273259" ID="Freemind_Link_1273373848" MODIFIED="1243831278167" TEXT="Networking"/>
<node CREATED="1243831279171" ID="Freemind_Link_1519217875" MODIFIED="1243831282351" TEXT="AI"/>
<node CREATED="1243831282947" ID="Freemind_Link_947857229" MODIFIED="1243831287055" TEXT="Sound"/>
<node CREATED="1243831287635" ID="Freemind_Link_195329637" MODIFIED="1243831289599" TEXT="Voice"/>
</node>
<node CREATED="1243830491203" ID="Freemind_Link_496350868" MODIFIED="1245389866859" POSITION="left" TEXT="Components factory based creation easy serialization/unserialization only data ">
<cloud/>
</node>
<node CREATED="1243830876475" ID="Freemind_Link_1308171526" MODIFIED="1243830959354" POSITION="right" TEXT="Entity Manager&#xa;Entity Factory&#xa;Event Triggers&#xa;">
<cloud/>
<node COLOR="#990000" CREATED="1243830975183" ID="Freemind_Link_1444178110" MODIFIED="1243831476827" TEXT="Accessing and using compenent data from Entities/System Interfaces">
<icon BUILTIN="pencil"/>
</node>
</node>
<node CREATED="1245389916671" ID="Freemind_Link_1217635570" MODIFIED="1245390583406" POSITION="left" TEXT="Application Level">
<cloud/>
<node CREATED="1245390159109" ID="Freemind_Link_1822970892" MODIFIED="1245390211750" TEXT="Separation from SDL and app"/>
<node CREATED="1245390212687" ID="Freemind_Link_1725001967" MODIFIED="1245392385671" TEXT="Support for wii, ps3, mac, iphone, android"/>
<node CREATED="1245390230531" ID="Freemind_Link_496357299" MODIFIED="1245390314093" TEXT=" Abstract our own threading so we can support mac"/>
<node CREATED="1245390316000" ID="Freemind_Link_445185886" MODIFIED="1245390420750" TEXT="Messaging layer but SDL support initially"/>
<node CREATED="1245390436343" ID="Freemind_Link_199887746" MODIFIED="1245390491375" TEXT="Start out with simple app w/sdl window and hit esc to close"/>
<node CREATED="1245390495343" ID="Freemind_Link_1122247318" MODIFIED="1245390539171" TEXT="Keep tests for regression"/>
<node CREATED="1245392670859" ID="Freemind_Link_1549454247" MODIFIED="1245392738265" TEXT="@ fode level, expose cpps or no"/>
</node>
<node CREATED="1245392915750" ID="Freemind_Link_480572909" MODIFIED="1245392922062" POSITION="right" TEXT="Standards">
<node CREATED="1245392930312" ID="Freemind_Link_1298048093" MODIFIED="1245392960921" TEXT="ain no void as func arg if no params"/>
</node>
</node>
</map>

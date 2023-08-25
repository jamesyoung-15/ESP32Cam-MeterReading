let getFormattedTime = () => {
    // Set timezone specfically to HK, output looks like: 25/08/2023, 11:04:44
    let timeNow = new Date().toLocaleString("en-GB", {timeZone: "Asia/Hong_Kong"});
    // console.log(timeNow);

    // Split the string to date and time like: 25/08/2023 and 11:04:44
    let splitTime = timeNow.split(" ");
    // console.log(splitTime);

    // split the date to month day year
    let regex = splitTime[0].match(/(\d{1,2})\/(\d{1,2})\/(\d{4})/);
    let newDateFormat = regex[1] + regex[2] + regex[3];

    // replace colon in time
    let newTimeFormat = splitTime[1].replace(/:/g, "");
    // console.log(newTimeFormat);

    // combine to form file name, eg. 25082023-110808
    let fileName = newDateFormat + "-" + newTimeFormat;
    // console.log(fileName);
    return fileName;
    
}
console.log(getFormattedTime());